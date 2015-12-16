/*
	Author Lorenzo Massimo Gramola AA 2015/1016
	http://lists.gnu.org/archive/html/help-bison/2009-03/msg00015.html

*/
%{
	#include <stdio.h>
	#include <stdlib.h> //exit
	#include <math.h>
	#include <string.h> //strncmp
	#include "header.h"
	int yylex(void);
	type *composeType(array*,const enum data_type);
	void yyerror(char const *);
	char * name_temp;
	enum data_type actual_type_value = -1;
	int inside_record = 0;
	symrec * tempTable;
%}

%union {
	double dbl;
	symrec *srec;
	type *tp;
	array *at;
	ref * rf;
	LRhand * H;
	record * rec;
	enum data_type dtype;
};
%start P

%token INT FLOAT
%token <dbl>  NUM
%token <srec> VAR
%token <rec> RECORD
%nonassoc PRINT
%nonassoc SEMICOLON LBRACK RBRACK LCURLY RCURLY RECORD DOT


%type <dbl> expr
%type <rf>Cvar
%type <dtype> B
%type <at> C
%type <tp> T
%type <H> L R


%error-verbose

%%

P: opt_dec_list LCURLY opt_cmd_list RCURLY;

opt_dec_list: 	/*empty*/
				| D 																									{
																															}
				;

opt_cmd_list:	/*empty*/
				| cmd_list
				;
cmd_list:		cmd SEMICOLON
				| cmd SEMICOLON cmd_list
				;
cmd:		/*empty*/
				| L '=' R  																						{
																																	int n = $3->value.num;
																																	int res = assignment($1,$3);
																																	if(res == 0){
																																		printf("Assignment exception.\nPlease check the code at line %d column %d\n", @2.first_line,@2.first_column);
																																		exit(0);
																																	}
																																}
				| PRINT expr																						{
																																	printf("%f\n", $2);
																																	//printf("%d\n", (yyvsp[(2) - (3)].dbl));
																																}
	 			;

expr: 	R 																											{
																																		$$ = Revaluate($1);
																																}
				;
L: 			VAR 																										{
																																	if(checkExistance($1, NULL) == 0){
																																		printf("access to %s which is not declared as a varibale.", $1->name);
																																		printf("Lvalue evaluation exception.\nPlease check the code at line %d column %d\n", @1.first_line,@1.first_column);
																																		exit(-1);
																																	}
																																}
							Cvar																							{
																																	if($3 != NULL){
																																		$1->ref = (ref*)malloc(sizeof(ref));
																																	}
																																	$1->ref = $3;
																																	//controllo istantaneo - se Cvar è un tipo di array controlliamo che
																																	// gli indici non eccedano quelli dichiarati per quella variabile
																																	// sempre che quella variabile sia un array
																																	if(typeChecking($1) == 0){
																																		printf("Unexpected error during type checking at line %d. ", @1.first_line);
																																		exit(-1);
																																	}
																																	LRhand *L = (LRhand*)malloc(sizeof(LRhand));
																																	L->type = type_VAR;
																																	L->value.rec = $1;
																																	$$ = L;
																															}
				| VAR DOT VAR 																				{
																																int res = validateStructAccess($1->name, $3->name);
																																if(res == 0){
																																	printf("Undefined Sruct access exception.\nPlease check the code at line %d column %d\n", @2.first_line,@2.first_column);
																																	exit(0);
																															}
																																LRhand *l = (LRhand*)malloc(sizeof(LRhand));
																																l->type = type_STRUCT;
																																l->value.rec = $1;
																																l->member.name = $3;
																																$$ = l;
																															}
				;
R :			VAR
																																{
																																	if(checkExistance($1, NULL) == 0){
																																		printf("access to %s which is not declared as a varibale.", $1->name);
																																		printf("Rvalue evaluation exception.\nPlease check the code at line %d column %d\n", @1.first_line,@1.first_column);
																																		exit(-1);
																																	}
																																}

				Cvar 																										{
																																if($3 == NULL){
																																	$1->ref = NULL;
																																}
																																else{
																																	$1->ref = $3;
																																}
																																int res = typeChecking($1);
																																if(res == 0){
																																	printf("Unexpected error in typeChecking at line %d. ", @1.first_line);
																																	exit(-1);
																																}
																																LRhand *R = (LRhand*)malloc(sizeof(LRhand));
																																R->type = type_VAR;
																																R->value.rec = $1;
																																$$ = R;
																															}
				| NUM																									{
																																$<dbl>$ = $1; // default we can skip it
																																LRhand *R = (LRhand*)malloc(sizeof(LRhand));
																																R->type = type_NUM;
																																R->value.num = $1;
																																int n = R->value.num;
																																$$ = R;
																															}
				| VAR DOT VAR 																				{
																																int res = validateStructAccess($1->name, $3->name);
																																if(res == 0){
																																	printf("Undefined Sruct access exception.\nPlease check the code at line %d column %d\n", @2.first_line,@2.first_column);
																																	exit(0);
																																}
																																LRhand *r = (LRhand*)malloc(sizeof(LRhand));
																																r->type = type_STRUCT;
																																r->value.rec = $1;
																																r->member.name = $3;
																																$$ = r;
																															}
				;

Cvar: 	/*empty*/																							{
																																$$ = NULL;
																															}
		| LBRACK NUM RBRACK Cvar 																	{
																																ref *res = (ref*)malloc(sizeof(ref));
																																//perform some check on i - our input have just test purposes
																																int i = $2;
																																res->n = i;
																																res->next = $4;
																																$$ = res;
																															}
		;
D: 	 T VAR SEMICOLON D 																				{
																																if(inside_record){
																																	$2 = createSymStruct($2->name,$1,&tempTable);
																																}else{
																																	$2 = createSym($2->name,$1);
																																}
																															}
	| T VAR SEMICOLON																						{
																																if(inside_record){ //what about using starting conditions? =P
																																	$2 = createSymStruct($2->name,$1,&tempTable);
																																}else{
																																	$2 = createSym($2->name,$1);
																																}
																															}
	;


T:	B																												{
																																actual_type_value = $1;
																														}
	C 																												{
																																$$ = composeType($3,$1);
																														}
	| RECORD 																									{
																																inside_record = 1;
																																tempTable = $1->tabella;
																														}
		LCURLY D RCURLY																					{
																															type * t = (type*)malloc(sizeof(type));
																															t->datatype = dt_structure;
																															t->value.r = $1;
																															$1->tabella = tempTable;
																															inside_record=0;
																															tempTable = NULL;
																															$$ = t;
																													}
	;
B: INT 																										{
																													}
	| FLOAT																									{
																													}
	;

C:	/*empty*/																							{
																														$$ = NULL;
																													}
	| LBRACK NUM RBRACK C 																	{
																														int n = $2;
																														if(n < 1){
																															printf("Parser refuses to parse arrays with less than 1 items or with negative values\nError occured at line %d column %d\n", @2.first_line,@2.first_column);
																															exit(0);
																														}
																														$$ = makeArray($4,n, actual_type_value);
																													}
	;

%%


symrec *sym_table;

int
main (void)
{
  yylloc.first_line = yylloc.last_line = 1;
  yylloc.first_column = yylloc.last_column = 0;
  return yyparse ();
}
