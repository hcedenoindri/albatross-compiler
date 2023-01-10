%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "ast.h"

#define UNUSED(x) ((void)x)

extern program p;

int yylex(void); /* function prototype */

void yyerror(const char *s) {
    fprintf(stderr,"parsing error %s\n", s); exit(1);
}

%}


%union {
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
	}

%define parse.error verbose

%token <ival> INT
       <name> NAME
       <name> TYPE
       <name> STRING

%token
  RETURN VAR IF ELSE WHILE OTHERWISE REPEAT
  LP RP LCB RCB LB RB COMMA SEMICOLON ASSIGN

%left  ASSIGN
%left  OR
%left  AND
%left  BOR
%left  XOR
%left  BAND
%left  EQ NE
%left  GT GE LT LE
%left  PLUS MINUS
%left  MUL DIV REM
%right NOT
%left  FUN

%type <vars>    vars
%type <var>     var
%type <funs>    funs
%type <fun>     fun
%type <stmts>   stmts
%type <stmt>    stmt
%type <exp>     exp
%type <args>    args
%type <arg>     arg
%type <ar>      ar
%type <ars>     ars

%start program

%%

program: vars funs stmts                                    { p.variables = $1; p.functions = $2; p.statements = $3; }

vars: vars var                                              { $$ = ListAddLast($2, $1); }
    | /* null */                                            { $$ = NULL; }

var: VAR NAME TYPE ASSIGN exp SEMICOLON                     { $$ = VarDecNode($2, TyNode($3), $5); }

funs: funs fun                                              { $$ = ListAddLast($2, $1); }
    | /* null */                                            { $$ = NULL; }

fun: FUN NAME TYPE LP ars RP LCB vars stmts RCB             { $$ = FunDecNode($2, TyNode($3), $5, $8, $9); }

ars: ar                                                     { $$ = $1; }
   | /* null */                                             { $$ = NULL; }

ar: NAME TYPE                                               { $$ = ListAddLast(Param(TyNode($2), $1), NULL); }
  | NAME TYPE COMMA ar                                      { $$ = ListAddFirst(Param(TyNode($2), $1), $4); }

stmts: stmt                                                 { $$ = ListAddLast($1, p.statements); }
     | stmts stmt                                           { $$ = ListAddLast($2, $1); }

stmt: RETURN exp SEMICOLON                                  { $$ = RetNode($2); }
    | RETURN SEMICOLON                                      { $$ = RetNode(NULL); }
    | IF LP exp RP LCB stmts RCB                            { $$ = IfNode($3, $6, NULL); }
    | IF LP exp RP LCB stmts RCB ELSE LCB stmts RCB         { $$ = IfNode($3, $6, $10); }
    | WHILE LP exp RP LCB stmts RCB                         { $$ = WhileNode($3, $6, NULL); }
    | WHILE LP exp RP LCB stmts RCB OTHERWISE LCB stmts RCB { $$ = WhileNode($3, $6, $10); }
    | REPEAT LP exp RP LCB stmts RCB                        { $$ = RepeatNode($3, $6); }
    | NAME ASSIGN exp SEMICOLON                             { $$ = AssignNode($1, $3); }
    | NAME LP args RP SEMICOLON                             { $$ = CallOpStmtNode($1, $3); }

exp: INT                                                    { $$ = IntNode($1); }
   | STRING                                                 { $$ = StringNode($1); }
   | exp NE exp                                             { $$ = BinOpNode(ne_op, $1, $3); }
   | exp EQ exp                                             { $$ = BinOpNode(eq_op, $1, $3); }
   | exp PLUS exp                                           { $$ = BinOpNode(plus_op, $1, $3); }
   | exp MINUS exp                                          { $$ = BinOpNode(minus_op, $1, $3); }
   | exp MUL exp                                            { $$ = BinOpNode(times_op, $1, $3); }
   | exp DIV exp                                            { $$ = BinOpNode(div_op, $1, $3); }
   | exp REM exp                                            { $$ = BinOpNode(rem_op, $1, $3); }
   | exp GT exp                                             { $$ = BinOpNode(gt_op, $1, $3); }
   | exp GE exp                                             { $$ = BinOpNode(ge_op, $1, $3); }
   | exp LT exp                                             { $$ = BinOpNode(lt_op, $1, $3); }
   | exp LE exp                                             { $$ = BinOpNode(le_op, $1, $3); }
   | exp XOR exp                                            { $$ = BinOpNode(xor_op, $1, $3); }
   | exp BAND exp                                           { $$ = BinOpNode(ban_op, $1, $3); }
   | exp BOR exp                                            { $$ = BinOpNode(bor_op, $1, $3); }
   | exp AND exp                                            { $$ = BinOpNode(and_op, $1, $3); }
   | exp OR exp                                             { $$ = BinOpNode(or_op, $1, $3); }
   | NOT exp                                                { $$ = UnOpNode(not_op, $2); }
   | NAME                                                   { $$ = VarOpNode($1) ; }
   | NAME LP args RP                                        { $$ = CallOpNode($1, $3); }
   | LP exp RP                                              { $$ = $2; }

args: /* null */                                            { $$ = NULL; }
    | arg                                                   { $$ = $1; }

arg: exp                                                    { $$ = ListAddLast($1, NULL); }
   | exp COMMA arg                                          { $$ = ListAddFirst($1, $3); }
