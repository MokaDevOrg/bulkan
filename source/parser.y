%{
	#include <iostream>
	#include <string>
	#include <memory>

	int yydebug = 0;

	extern int yylex();

	void yyerror(const char * str)
	{
		std::cerr << "error: " << str << std::endl;
	}
%}

%union {
	float number;
	std::string * id;
}

%token LET PLUS MULT DIV INTDIV MOD EXP SUB PLUS_MINUS
%token <id> ID
%token <number> NUMBER

%left PLUS SUB
%left MULT DIV INTDIV MOD
%left EXP
%right ID '('

%start program

%%

program:
	// empty
	| program function
	;

function:
	ID '(' parameterList ')' '=' statementList ';'
	;

parameterList:
	// empty
	| parameterList ',' ID
	| parameterList ',' NUMBER
	| parameterList ',' epsilon
	| ID
	| NUMBER
	| epsilon
	;

epsilon:
	NUMBER PLUS_MINUS NUMBER
	;

statementList:
	// empty
	| statementList statement
	;

statement:
	lambda
	| variableDeclaration
	| assignment
	| expressionStatement
	;

lambda:
	LET ID '(' parameterList ')' '=' expression
	;

variableDeclaration:
	LET ID '=' expression
	;

assignment:
	ID '=' expression
	;

expressionStatement:
	expression
	;

expression:
	NUMBER
	| ID
	| functionCall
	| expression PLUS expression
	| expression SUB expression
	| expression MULT expression
	| expression DIV expression
	| expression EXP expression
	| expression INTDIV expression
	| expression MOD expression
	| '(' expression ')'
	;

functionCall:
	ID '(' expressionList ')'
	;

expressionList:
	// empty
	| expressionList expression
	;
