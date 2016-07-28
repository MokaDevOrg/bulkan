%{
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../ast.hpp"

size_t yyline = 1;

extern int yylex();

extern std::vector<Function*> functions;

void yyerror(const char * str)
{
	std::cerr << "error: " << str << " at line " << yyline << std::endl;
	exit(1);
}
%}

%error-verbose

%union {
	Function * function;
	Parameter * parameter;
	Statement * statement;
	Expression * expression;
	ListId * listId;

	std::vector<std::shared_ptr<Parameter>> * parameterList;
	std::vector<IdParameter> * idParameterList;
	std::vector<std::shared_ptr<Statement>> * statementList;
	std::vector<std::shared_ptr<Expression>> * expressionList;

	float number;
	std::string * id;
}


%token LET PRINT ASSERT EQUALS PLUS MULT DIV INTDIV MOD EXP SUB PLUS_MINUS DO FOR RANGE RANGE_EX
%token <id> ID STRING
%token <number> NUMBER


%type <function> function
%type <listId> listId
%type <parameter> epsilon parameter
%type <statement> statement assignment lambda variableDecl expressionStatement print assert for
%type <expression> expression functionCall list
%type <parameterList> parameterList
%type <statementList> statementList
%type <expressionList> expressionList
%type <idParameterList> idParameterList


%left EQUALS NOT_EQUALS
%left LESS_EQUAL GREATER_EQUAL '<' '>'
%left PLUS SUB
%left MULT DIV INTDIV MOD
%left EXP

%right ID '(' '['

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
		$$ = new Function(*$1, *$3, Block(*$6, true));
	}
	;

parameterList:
	// empty
	{
		$$ = new std::vector<std::shared_ptr<Parameter>>();
	}
	| parameterList ',' parameter
	{
		$1->push_back(std::shared_ptr<Parameter>($3));
	}
	| parameter
	{
		$$ = new std::vector<std::shared_ptr<Parameter>>();
		$$->push_back(std::shared_ptr<Parameter>($1));
	}
	;

parameter:
	ID
	{
		$$ = new IdParameter(*$1);
	}
	| NUMBER
	{
		$$ = new NumberParameter($1);
	}
	| epsilon
	{
		$$ = $1;
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
	| print
	| assert
	| for
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
		$$ = new VariableDecl(*$2, std::shared_ptr<Expression>($4));
	}
	| LET listId '=' expression
	{
		$$ = new ListDecl(std::shared_ptr<ListId>($2), std::shared_ptr<Expression>($4));
	}
	| LET listId
	{
		$$ = new ListDecl(std::shared_ptr<ListId>($2));
	}
	;

assignment:
	ID '=' expression
	{
		$$ = new Assignment(*$1, std::shared_ptr<Expression>($3));
	}
	;

expressionStatement:
	expression
	{
		$$ = new ExpressionStatement(std::shared_ptr<Expression>($1));
	}
	;

print:
	PRINT expression
	{
		$$ = new Log(std::shared_ptr<Expression>($2));
	}
	| PRINT STRING
	{
		$$ = new Log(*$2);
	}
	;

assert:
	ASSERT expression STRING
	{
		$$ = new Assert(std::shared_ptr<Expression>($2), *$3);
	}
	;

for:
	FOR ID ':' expression RANGE expression DO statementList ';'
	{
		$$ = new For(*$2, std::shared_ptr<Expression>($4), std::shared_ptr<Expression>($6),
				Block(*$8, false), true);
	}
	| FOR ID ':' expression RANGE_EX expression DO statementList ';'
	{
		$$ = new For(*$2, std::shared_ptr<Expression>($4), std::shared_ptr<Expression>($6),
				Block(*$8, false), false);
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
	| expression EQUALS expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "==", std::shared_ptr<Expression>($3));
	}
	| expression NOT_EQUALS expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "!=", std::shared_ptr<Expression>($3));
	}
	| expression LESS_EQUAL expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "<=", std::shared_ptr<Expression>($3));
	}
	| expression GREATER_EQUAL expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), ">=", std::shared_ptr<Expression>($3));
	}
	| expression '<' expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), "<", std::shared_ptr<Expression>($3));
	}
	| expression '>' expression
	{
		$$ = new BinaryOp(std::shared_ptr<Expression>($1), ">", std::shared_ptr<Expression>($3));
	}
	| '(' expression ')'
	{
		$$ = $2;
	}
	| listId
	{
		$$ = new ListElement(std::shared_ptr<ListId>($1));
	}
	| list
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

list:
	'[' expressionList ']'
	{
		
	}
	;

listId:
	ID '[' expressionList ']'
	{
		$$ = new ListId(*$1, *$3);
	}
	;
