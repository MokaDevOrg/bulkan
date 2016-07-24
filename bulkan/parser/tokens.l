%{
	#include <iostream>
	#include <string>
	#include <vector>
	
	#include "../ast.hpp"
	#include "../gen/parser.hpp"
	#include "../Registry.hpp"
	
	extern size_t yyline;
	
	#define TOKEN(X) { return X; }
%}

%option noyywrap

%x S_COMMENT
%x S_STRING

%%
let						TOKEN(LET);
log						TOKEN(LOG);
assert					TOKEN(ASSERT);
[0-9]+(\.[0-9]*)?      	{ yylval.number = std::stof(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]*   	{ yylval.id = registry.get(yytext, yyleng); return ID; }
==						TOKEN(EQUALS);
!=						TOKEN(NOT_EQUALS);
\<=						TOKEN(LESS_EQUAL);
\>=						TOKEN(GREATER_EQUAL);
\+						TOKEN(PLUS);
-						TOKEN(SUB);
\*						TOKEN(MULT);
\/\/					TOKEN(INTDIV);
\/						TOKEN(DIV);
\^						TOKEN(EXP);
%						TOKEN(MOD);
\'                     	BEGIN(S_STRING);
#						BEGIN(S_COMMENT);

\n                     	/* ignore EOL */ yyline++;
[ \t]+                 	/* ignore whitespace */;

.						return (int) yytext[0];

<S_COMMENT>[^\n]*      	BEGIN(INITIAL);

<S_STRING>[^\']*       	{ yylval.id = registry.get(yytext, yyleng); TOKEN(STRING); }
<S_STRING>\'           	BEGIN(INITIAL);
%%
