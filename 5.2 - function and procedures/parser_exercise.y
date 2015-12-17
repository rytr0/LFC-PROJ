/** calculator calc.y
 * revision Lorenzo Massimo Gramola (2015)
*/

%{
    #include <stdio.h>
    int yylex(void);
    void yyerror(char *s);
    extern FILE* yyin;
    char ** fileList;
    unsigned currentFile = 0;
    unsigned nFiels;
%}
%error-verbose

%token INTEGER
%token VARIABLE
%token WHILE IF PRINT FOR TO
%nonassoc IFX
%nonassoc ELSE
%nonassoc CONST RECORD

%left GE LE EQ NE LT GT
%left PLUS MINUS
%left MULTIPLY DIVIDE
%nonassoc LP RP
%nonassoc UMINUS RCURLY LCURLY COMMA SEMICOLON  INT FLOAT  MAIN RBRACK LBRACK BOOL



%%

program:
  opt_dec_list
  opt_proc_list
  MAIN
  opt_stmt_list                                                                 {printf("%s\n", "parsed");}
opt_dec_list:
        /*empty*/
        | dec_list

dec_list:
    dec
    | dec_list dec
    ;

dec:
    type VARIABLE SEMICOLON
    | type CONST VARIABLE '=' expr
    | LCURLY opt_dec_list RCURLY VARIABLE
    ;

type: B
      | C
      | RECORD
      ;

C : /*EmptY*/
    | LBRACK INTEGER RBRACK C;

opt_stmt_list:
      /*empty*/
      | stmt_list
      ;
opt_proc_list:
      /*empty*/
      | proc_list
      ;

proc_list:
        proc
        | proc_list proc
        ;
proc:
        opt_type VARIABLE LP form RP LCURLY
                opt_stmt_list
            RCURLY
        ;
opt_type:
        /*empty*/
        | B
        ;
B:      INT
        | FLOAT
        | BOOL
        ;
form:
         /*empty*/
         |form_list
         ;
form_list:
        param
        | form COMMA param
        ;
param:  B VARIABLE
        ;
opt_actual_expr:
        /*empty*/
        |actual_expr
        ;
actual_expr:
        expr
        |actual_expr COMMA expr
        ;
stmt:
        SEMICOLON
        | expr SEMICOLON
        | PRINT expr SEMICOLON
        | VARIABLE '=' expr SEMICOLON
        | WHILE LP expr RP stmt
        | IF LP expr RP stmt %prec IFX
        | IF LP expr RP stmt ELSE stmt
        | FOR LP VARIABLE '=' expr TO expr RP stmt
        | LCURLY stmt_list RCURLY
        ;


stmt_list:
        stmt
        | stmt_list stmt
        ;
expr:
        INTEGER
        | FLOAT
        | VARIABLE
        | MINUS expr %prec UMINUS
        | expr PLUS expr
        | expr MINUS expr
        | expr MULTIPLY expr
        | expr DIVIDE expr
        | expr LT expr
        | expr GT expr
        | expr GE expr
        | expr LE expr
        | expr NE expr
        | expr EQ expr
        | LP expr RP
        | VARIABLE LP
            opt_actual_expr RP
        ;

%%


void yyerror(char *s){
    fprintf(stdout,"%s\n",s);
}

int yywrap(){
	//from lex and yacc o'reilly
	//When yylex() reaches the end of its input file, it calls yywrap(),which￼returns a value of 0 or 1.
	//If the value is 1,the program is done and there is no more input.
	//If the value is 0, on the other hand, the lexer assumes that yywrap() has opened another file for it to read, and continues to read from yyin.
	//The default yywrap() always returns 1. By providing our own ver- sion of yywrap(), we can have our program read all of the files named on the command line, one at a time.
	FILE *file;

	if(yyin!=NULL){
		//close the file
		fclose(yyin);
	}

	file = NULL;
	//posso prenderlo anche se currentFile è zero perchè punto già al primo elemento della lista di file
	if(fileList[currentFile] != (char*)0){
		file = fopen(fileList[currentFile], "r");
		if(file!= NULL){
			currentFile++;
			yyin = file;
		}
	}
	if(file == NULL && currentFile < nFiels){
		fprintf(stderr,"%s %s\n","could not open file",fileList[currentFile]);
	}
	return (file ? 0 : 1); //1 = no more input
}

int main(int argc, char ** argv){
	if(argc < 2){
		fprintf(stderr,"%s\n","Usage is compiled_file input_file_list");
		return -1;
	}
	//puntatore al primo file di input
	fileList = argv+1;
	nFiels = argc-1;
	//open the files and set it to yyin
	yywrap();
	//parse the file to produce output/tokens
	yyparse();
	return 0;
}
