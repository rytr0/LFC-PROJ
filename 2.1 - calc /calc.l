/** calculator calc.l
 * origin source by Tom Niemann at epaperpress.com/lexandyacc
 * revision Lorenzo Massimo Gramola (2014)
 * revision Lorenzo Massimo Gramola (2015)
 */
%{
    #include "y.tab.h"
    #include <stdlib.h>
    void yyerror(char *);
%}

%%
 /*
 * managing variables,
 * NOTE: variable with a single char name
 */
[a-z]   {
        yylval = *yytext - 'a'; 
        return VARIABLE;
        }
 /**
 * managing integers
 */
[0-9]+      {
                yylval = atoi(yytext);
                return INTEGER;
            }
 /**
 * managing operators
 */
[-+()=/*\n]      { return *yytext; }

[ \t]       ;       /* skip whitespace */

 /**
 * everything else matched correspond to an error
 * the dot match anycharacter except the new line
 */
.           yyerror("Unknown character");

%%

int yywrap(void) {
    return 1;
}
