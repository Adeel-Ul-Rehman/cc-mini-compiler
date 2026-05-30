%{
#include "parser.h"
#include <string.h>
%}

%option noyywrap

%%

"void"              { return TYPE_VOID; }
"int"               { return TYPE_INT; }
"float"             { return TYPE_FLOAT; }
"bool"              { return TYPE_BOOL; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"return"            { return RETURN; }
"input"             { return INPUT; }
"output"            { return OUTPUT; }
"true"              { yylval.ival = 1; return BOOL_LIT; }
"false"             { yylval.ival = 0; return BOOL_LIT; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.sval = strdup(yytext); return IDENTIFIER; }
[0-9]+              { yylval.ival = atoi(yytext); return INT_LIT; }
[0-9]+"."[0-9]+     { yylval.fval = atof(yytext); return FLOAT_LIT; }
\"[^\"]*\"          { yylval.sval = strdup(yytext); return STRING_LIT; }
"=="                { return EQ; }
"!="                { return NE; }
"<="                { return LE; }
">="                { return GE; }
"&&"                { return AND; }
"||"                { return OR; }
"="                 { return ASSIGN; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return SLASH; }
"<"                 { return LT; }
">"                 { return GT; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
[ \t\n\r]+          { /* skip */ }
.                   { return yytext[0]; }

%%