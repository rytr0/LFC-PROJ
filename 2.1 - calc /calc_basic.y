/** calculator calc.y
 * origin source by Tom Niemann at epaperpress.com/lexandyacc
 * revision Lorenzo Massimo Gramola (2014)
 * revision Lorenzo Massimo Gramola (2015) 
 */

%{
    #include <stdio.h>
    int yylex(void);
    void yyerror(char *);
%}

/*
 * we specify here the token used, in order for YACC TO create
 * the pieces of codes like '#DEFINE...' in the y.tab.h
 * further more we specify the precedence for the operators - omitted on purpose in this file -> we get the S/R conflict
 */

%token INTEGER VARIABLE

%{
    void yyerror(char *);
    int yylex(void);
%}

%left '-' '+'
%%

/**
 we change a little bit the program structure here
 grammar becomes more interesting
 */

program:
        expr '\n'                 {printf("%d\n", $1);}
        | 
        ;

expr:
        INTEGER
        | expr '+' expr           { $$ = $1 + $3; }
        | expr '-' expr           { $$ = $1 - $3; }
        ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
