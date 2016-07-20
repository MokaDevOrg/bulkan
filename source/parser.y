%{
#include <iostream>
#include <string>
#include <vector>

#include "ast.hpp"

int yydebug = 0;

extern int yylex();

std::vector<Function> functions;

void yyerror(const char * str)
{
	std::cerr << "error: " << str << std::endl;
}
%}

%union {
	Function * function;
	Parameter * parameter;
	Statement * statement;
	Expression * expression;

	std::vector<Parameter> * parameterList;
	std::vector<Statement> * statementList;
	std::vector<Expression> * expressionList;

	float number;
	std::string * id;
}


%token LET PLUS MULT DIV INTDIV MOD EXP SUB PLUS_MINUS
%token <id> ID
%token <number> NUMBER


%type <function> function
%type <parameter> epsilon
%type <statement> statement assignment lambda variableDecl expressionStatement
%type <expression> expression functionCall
%type <parameterList> parameterList
%type <statementList> statementList
%type <expressionList> expressionList


%left PLUS SUB
%left MULT DIV INTDIV MOD
%left EXP
%right ID '('


%start program

%%

program:
	// empty
	| program function
	{
		functions.push_back(*$2);
	}
	;

function:
	ID '(' parameterList ')' '=' statementList ';'
	{
		$$ = new Function(*$1, *$3, Block(*$6));
	}
	;

parameterList:
	// empty
	{
		$$ = new std::vector<Parameter>();
	}
	| parameterList ',' ID
	{
		$1->push_back(IdParameter(*$3));
	}
	| parameterList ',' NUMBER
	{
		$1->push_back(NumberParameter($3));
	}
	| parameterList ',' epsilon
	{
		$1->push_back(*$3);
	}
	| ID
	{
		$$ = new std::vector<Parameter>();
		$$->push_back(IdParameter(*$1));
	}
	| NUMBER
	{
		$$ = new std::vector<Parameter>();
		$$->push_back(NumberParameter($1));
	}
	| epsilon
	{
		$$ = new std::vector<Parameter>();
		$$->push_back(*$1);
	}
	;

epsilon:
	NUMBER PLUS_MINUS NUMBER
	{
		$$ = new EpsilonParameter($1, $3);
	}
	;

statementList:
	// empty
	{
		$$ = new std::vector<Statement>();
	}
	| statementList statement
	{
		$1->push_back(*$2);
	}
	;

statement:
	lambda
	| variableDecl
	| assignment
	| expressionStatement
	;

lambda:
	LET ID '(' parameterList ')' '=' expression
	{
		$$ = new Lambda(*$2, *$4, *$7);
	}
	;

variableDecl:
	LET ID '=' expression
	{
		$$ = new VariableDecl(*$2, *$4);
	}
	;

assignment:
	ID '=' expression
	{
		$$ = new Assignment(*$1, *$3);
	}
	;

expressionStatement:
	expression
	{
		$$ = new ExpressionStatement(*$1);
	}
	;

expression:
	NUMBER
	{
		$$ = new Number($1);
	}
	| ID
	{
		$$ = new Id(*$1);
	}
	| functionCall
	| expression PLUS expression
	{
		$$ = new BinaryOp(*$1, "+", *$3);
	}
	| expression SUB expression
	{
		$$ = new BinaryOp(*$1, "-", *$3);
	}
	| expression MULT expression
	{
		$$ = new BinaryOp(*$1, "*", *$3);
	}
	| expression DIV expression
	{
		$$ = new BinaryOp(*$1, "/", *$3);
	}
	| expression EXP expression
	{
		$$ = new BinaryOp(*$1, "^", *$3);
	}
	| expression INTDIV expression
	{
		$$ = new BinaryOp(*$1, "//", *$3);
	}
	| expression MOD expression
	{
		$$ = new BinaryOp(*$1, "%", *$3);
	}
	| '(' expression ')'
	{
		$$ = $2;
	}
	;

functionCall:
	ID '(' expressionList ')'
	{
		$$ = new FunctionCall(*$1, *$3);
	}
	;

expressionList:
	// empty
	{
		$$ = new std::vector<Expression>();
	}
	| expressionList expression
	{
		$1->push_back(*$2);
	}
	;
