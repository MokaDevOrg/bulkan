%{
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "ast.hpp"

int yydebug = 0;

extern int yylex();

extern std::vector<Function> functions;

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

	std::vector<std::shared_ptr<Parameter>> * parameterList;
	std::vector<IdParameter> * idParameterList;
	std::vector<std::shared_ptr<Statement>> * statementList;
	std::vector<std::shared_ptr<Expression>> * expressionList;

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
%type <idParameterList> idParameterList


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
		$$ = new std::vector<std::shared_ptr<Parameter>>();
	}
	| parameterList ',' ID
	{
		$1->push_back(std::make_shared<IdParameter>(*$3));
	}
	| parameterList ',' NUMBER
	{
		$1->push_back(std::make_shared<NumberParameter>($3));
	}
	| parameterList ',' epsilon
	{
		$1->push_back(std::shared_ptr<Parameter>($3));
	}
	| ID
	{
		$$ = new std::vector<std::shared_ptr<Parameter>>();
		$$->push_back(std::make_shared<IdParameter>(*$1));
	}
	| NUMBER
	{
		$$ = new std::vector<std::shared_ptr<Parameter>>();
		$$->push_back(std::make_shared<NumberParameter>($1));
	}
	| epsilon
	{
		$$ = new std::vector<std::shared_ptr<Parameter>>();
		$$->push_back(std::shared_ptr<Parameter>($1));
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
		$$ = new std::vector<std::shared_ptr<Statement>>();
	}
	| statementList statement
	{
		$1->push_back(std::shared_ptr<Statement>($2));
	}
	;

statement:
	lambda
	| variableDecl
	| assignment
	| expressionStatement
	;

lambda:
	LET ID '(' idParameterList ')' '=' expression
	{
		$$ = new Lambda(*$2, *$4, std::shared_ptr<Expression>($7));
	}
	;

idParameterList:
	// empty
	{
		$$ = new std::vector<IdParameter>();
	}
	| idParameterList ID
	{
		$1->push_back(IdParameter(*$2));
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
		$$ = new ExpressionStatement(std::shared_ptr<Expression>($1));
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
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "+", std::shared_ptr<Expression>($3));
	}
	| expression SUB expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "-", std::shared_ptr<Expression>($3));
	}
	| expression MULT expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "*", std::shared_ptr<Expression>($3));
	}
	| expression DIV expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "/", std::shared_ptr<Expression>($3));
	}
	| expression EXP expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "^", std::shared_ptr<Expression>($3));
	}
	| expression INTDIV expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "//", std::shared_ptr<Expression>($3));
	}
	| expression MOD expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "%", std::shared_ptr<Expression>($3));
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
		$$ = new std::vector<std::shared_ptr<Expression>>();
	}
	| expressionList ',' expression
	{
		$1->push_back(std::shared_ptr<Expression>($3));
	}
	| expression
	{
		$$ = new std::vector<std::shared_ptr<Expression>>();
		$$->push_back(std::shared_ptr<Expression>($1));
	}
	;
