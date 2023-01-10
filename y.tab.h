/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    NAME = 259,                    /* NAME  */
    TYPE = 260,                    /* TYPE  */
    STRING = 261,                  /* STRING  */
    RETURN = 262,                  /* RETURN  */
    VAR = 263,                     /* VAR  */
    IF = 264,                      /* IF  */
    ELSE = 265,                    /* ELSE  */
    WHILE = 266,                   /* WHILE  */
    OTHERWISE = 267,               /* OTHERWISE  */
    REPEAT = 268,                  /* REPEAT  */
    LP = 269,                      /* LP  */
    RP = 270,                      /* RP  */
    LCB = 271,                     /* LCB  */
    RCB = 272,                     /* RCB  */
    LB = 273,                      /* LB  */
    RB = 274,                      /* RB  */
    COMMA = 275,                   /* COMMA  */
    SEMICOLON = 276,               /* SEMICOLON  */
    ASSIGN = 277,                  /* ASSIGN  */
    OR = 278,                      /* OR  */
    AND = 279,                     /* AND  */
    BOR = 280,                     /* BOR  */
    XOR = 281,                     /* XOR  */
    BAND = 282,                    /* BAND  */
    EQ = 283,                      /* EQ  */
    NE = 284,                      /* NE  */
    GT = 285,                      /* GT  */
    GE = 286,                      /* GE  */
    LT = 287,                      /* LT  */
    LE = 288,                      /* LE  */
    PLUS = 289,                    /* PLUS  */
    MINUS = 290,                   /* MINUS  */
    MUL = 291,                     /* MUL  */
    DIV = 292,                     /* DIV  */
    REM = 293,                     /* REM  */
    NOT = 294,                     /* NOT  */
    FUN = 295                      /* FUN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define INT 258
#define NAME 259
#define TYPE 260
#define STRING 261
#define RETURN 262
#define VAR 263
#define IF 264
#define ELSE 265
#define WHILE 266
#define OTHERWISE 267
#define REPEAT 268
#define LP 269
#define RP 270
#define LCB 271
#define RCB 272
#define LB 273
#define RB 274
#define COMMA 275
#define SEMICOLON 276
#define ASSIGN 277
#define OR 278
#define AND 279
#define BOR 280
#define XOR 281
#define BAND 282
#define EQ 283
#define NE 284
#define GT 285
#define GE 286
#define LT 287
#define LE 288
#define PLUS 289
#define MINUS 290
#define MUL 291
#define DIV 292
#define REM 293
#define NOT 294
#define FUN 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "parser.y"

	int             ival;
	char*           name;
	list*           stmts;
    stmt_node*      stmt;
    list*           vars;
    vardec_node*    var;
    list*           funs;
    fundec_node*    fun;
    exp_node*       exp;
    list*           args;
    list*           arg;
    list*           ar;
    list*           ars;
	

#line 164 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
