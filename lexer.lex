%{

#include <string.h>
#include "ast.h"
#include "y.tab.h"

char buffer[10000];
int pos = 0;

%}

%option noyywrap

%x DEC
%x STR

%%

" "                     {  }
"\t"                    {  }
"\n"                    {  }

"#".*"\n"               {  }

[0-9]                   { BEGIN(DEC); buffer[0] = yytext[0]; pos = 1; }
<DEC>{
[0-9]                   { buffer[pos] = yytext[0]; pos += 1; }
[a-zA-Z]                { fprintf(stderr, "illegal token \n"); exit(1); }
.|"\n"                  { buffer[pos] = '\0'; yylval.ival = atoi(buffer); pos = 0; yyless(0); BEGIN(INITIAL); return INT; }
}

"var"                   { return VAR; }
"fun"                   { return FUN; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }
"otherwise"             { return OTHERWISE; }
"repeat"                { return REPEAT; }
"return"                { return RETURN; }

"("                     { return LP; }
")"                     { return RP; }
"{"                     { return LCB; }
"}"                     { return RCB; }
"["                     { return LB; }
"]"                     { return RB; }
","                     { return COMMA; }
";"                     { return SEMICOLON; }

"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"%"                     { return REM; }
"&"                     { return BAND; }
"|"                     { return BOR; }
"^"                     { return XOR; }

"&&"                    { return AND; }
"||"                    { return OR; }
"=="                    { return EQ; }
"<>"                    { return NE; }
"<"                     { return LT; }
"<="                    { return LE; }
">"                     { return GT; }
">="                    { return GE; }

"!"                     { return NOT; }

":="                    { return ASSIGN; }

"void"                  { yylval.name = "void"; return TYPE; }
"int"                   { yylval.name = "int"; return TYPE; }
"char"                  { yylval.name = "char"; return TYPE; }
"string"                { yylval.name = "string"; return TYPE; }

\"                      { BEGIN(STR); pos = 0; }
<STR>{
"\\n"                   { buffer[pos] = '\n'; pos += 1; }
"\\t"                   { buffer[pos] = '\t'; pos += 1; }
"\\\\"                  { buffer[pos] = '\\'; pos =+ 1; }
"\\\""                  { buffer[pos] = '"'; pos += 1; }
"\\".|"\n"              { fprintf(stderr, "illegal token\n"); exit(1);  }
\"                      { buffer[pos] = '\0'; yylval.name = strdup(buffer); BEGIN(INITIAL); return STRING; }
.                       { buffer[pos] = yytext[0]; pos += 1; }
}

[a-zA-Z][a-zA-Z0-9]*    { yylval.name = strdup(yytext); return NAME; }

.                     { fprintf(stderr, "illegal token\n"); exit(1); }
