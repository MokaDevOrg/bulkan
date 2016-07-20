%{
	#include <iostream>
	#include <string>
	#include <vector>
	
	#include "ast.hpp"

	int yydebug = 0;

	extern int yylex();

	extern std::vector<Function> functions;

	void yyerror(const char * str)
	{
		std::cerr << "error: " << str << std::endl;
	}
%}

%union SuperUnion {
	Function function;
	Parameter parameter;
	Statement assignment;
	Expression expression;
	
	std::vector<Parameter> parameterList;
	std::vector<Statement> statementList;
	std::vector<Expression> expressionList;
	
	float number;
	std::string * id;
	
	SuperUnion() {}
	~SuperUnion() {}
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
		functions.push_back($2);
	}
	;

function:
	ID '(' parameterList ')' '=' statementList ';'
	{
		$$ = Function($1, $3, Block($6));
	}
	;

parameterList:
	// empty
	{
		$$ = std::vector<Parameter>();
	}
	| parameterList ',' ID
	{
		$1.push_back(Id($3));
	}
	| parameterList ',' NUMBER
	{
		$1.push_back(Number($3));
	}
	| parameterList ',' epsilon
	{
		$1.push_back($3);
	}
	| ID
	{
		$$ = std::vector<Parameter>();
		$$.push_back(IdParameter($1));
	}
	| NUMBER
	{
		$$ = std::vector<Parameter>();
		$$.push_back(NumberParameter($1));
	}
	| epsilon
	{
		$$ = std::vector<Parameter>();
		$$.push_back($1);
	}
	;

epsilon:
	NUMBER PLUS_MINUS NUMBER
	{
		$$ = EpsilonParameter($1, $3);
	}
	;

statementList:
	// empty
	{
		$$ = std::vector<Statement>();
	}
	| statementList statement
	{
		$1.push_back($2);
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
		$$ = Lambda($2, $4, $7);
	}
	;

variableDecl:
	LET ID '=' expression
	{
		$$ = VariableDecl($2, $4);
	}
	;

assignment:
	ID '=' expression
	{
		$$ = Assignment($1, $3);
	}
	;

expressionStatement:
	expression
	{
		$$ = ExpressionStatement($1);
	}
	;

expression:
	NUMBER
	{
		$$ = Number($1);
	}
	| ID
	{
		$$ = Id($1);
	}
	| functionCall
	| expression PLUS expression
	{
		$$ = BinaryOp($1, "+", $3);
	}
	| expression SUB expression
	{
		$$ = BinaryOp($1, "-", $3);
	}
	| expression MULT expression
	{
		$$ = BinaryOp($1, "*", $3);
	}
	| expression DIV expression
	{
		$$ = BinaryOp($1, "/", $3);
	}
	| expression EXP expression
	{
		$$ = BinaryOp($1, "^", $3);
	}
	| expression INTDIV expression
	{
		$$ = BinaryOp($1, "//", $3);
	}
	| expression MOD expression
	{
		$$ = BinaryOp($1, "%", $3);
	}
	| '(' expression ')'
	{
		$$ = $2;
	}
	;

functionCall:
	ID '(' expressionList ')'
	{
		$$ = FunctionCall($1, $3);
	}
	;

expressionList:
	// empty
	{
		$$ = std::vector<Expression>();
	}
	| expressionList expression
	{
		$1.push_back($2);
	}
	;
