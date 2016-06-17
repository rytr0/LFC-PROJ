/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT_T = 258,
    FLOAT_T = 259,
    BOOL_T = 260,
    INTEGER_V = 261,
    BOOLEAN_V = 262,
    FLOAT_V = 263,
    VARIABLE = 264,
    WHILE = 265,
    IF = 266,
    PRINT = 267,
    FOR = 268,
    TO = 269,
    IFX = 270,
    ARGS = 271,
    ELSE = 272,
    CONST = 273,
    RECORD = 274,
    GE = 275,
    LE = 276,
    EQ = 277,
    NE = 278,
    LT = 279,
    GT = 280,
    PLUS = 281,
    MINUS = 282,
    MULTIPLY = 283,
    DIVIDE = 284,
    EQUALS = 285,
    UMINUS = 286,
    BYREF = 287,
    RCURLY = 288,
    LCURLY = 289,
    COMMA = 290,
    SEMICOLON = 291,
    MAIN = 292,
    RBRACK = 293,
    LBRACK = 294,
    EVAL = 295,
    LP = 296,
    RP = 297
  };
#endif
/* Tokens.  */
#define INT_T 258
#define FLOAT_T 259
#define BOOL_T 260
#define INTEGER_V 261
#define BOOLEAN_V 262
#define FLOAT_V 263
#define VARIABLE 264
#define WHILE 265
#define IF 266
#define PRINT 267
#define FOR 268
#define TO 269
#define IFX 270
#define ARGS 271
#define ELSE 272
#define CONST 273
#define RECORD 274
#define GE 275
#define LE 276
#define EQ 277
#define NE 278
#define LT 279
#define GT 280
#define PLUS 281
#define MINUS 282
#define MULTIPLY 283
#define DIVIDE 284
#define EQUALS 285
#define UMINUS 286
#define BYREF 287
#define RCURLY 288
#define LCURLY 289
#define COMMA 290
#define SEMICOLON 291
#define MAIN 292
#define RBRACK 293
#define LBRACK 294
#define EVAL 295
#define LP 296
#define RP 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 43 "parser.y" /* yacc.c:1909  */

    int iValue;
    float fValue;
    int bValue;
    treeNode * tNode;
    symrec * symbolRecord;
    basicType bType;
    type * t;
    program * prg;
    form * formal;
    actual * ae;
    routine * routine;
    list * list;

#line 153 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
