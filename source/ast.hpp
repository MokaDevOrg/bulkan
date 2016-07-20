#pragma once

#include <vector>

typedef double NUMBER_T;

class Parameter {};
class Statement {};
class Expression {};

class Block
{
public:
	std::vector<Statement> statements;
	
	Block(std::vector<Statement> statements) :
		statements(statements) {}
};

class Function
{
public:
	std::string name;
	std::vector<Parameter> parameters;
	Block block;

	Function(std::string name, std::vector<Parameter> parameters, Block block) :
		name(name), parameters(parameters), block(block) {}
};



class IdParameter : Parameter
{
public:
	std::string name;
	
	IdParameter(std::string name) :
		name(name) {}
};

class NumberParameter : Parameter
{
public:
	NUMBER_T value;
	
	NumberParameter(NUMBER_T value):
		value(value) {}
};

class EpsilonParameter : NumberParameter
{
public:
	NUMBER_T epsilon;
	
	EpsilonParameter(NUMBER_T value, NUMBER_T epsilon) :
		NumberParameter(value), epsilon(epsilon) {}
};


class Lambda : Statement
{
public:
	std::string name;
	std::vector<Parameter> parameters;
	Expression expression;
	
	Lambda(std::string name, std::vector<Parameter> parameters, Expression expression) :
		name(name), parameters(parameters), expression(expression) {}
};

class VariableDecl : Statement
{
public:
	std::string name;
	Expression expression;
	
	VariableDecl(std::string name, Expression expression) :
		name(name), expression(expression) {}
};

class Assignment : Statement
{
public:
	std::string name;
	Expression expression;
	
	Assignment(std::string name, Expression expression) :
		name(name), expression(expression) {}
};


class ExpressionStatement : Statement
{
public:
	Expression expression;
	
	ExpressionStatement(Expression expression) :
		expression(expression) {}
};

class Number : Expression
{
public:
	NUMBER_T value;
	
	Number(NUMBER_T value) :
		value(value) {}
};

class Id : Expression
{
public:
	std::string name;
	
	Id(std::string name) :
		name(name) {}
};

class FunctionCall : Expression
{
public:
	std::string value;
	std::vector<Expression> expressionList;
	
	FunctionCall(std::string value, std::vector<Expression> expressionList) :
		value(value), expressionList(expressionList) {}
};

class BinaryOp : Expression
{
public:
	Expression left;
	Expression right;
	std::string op;
	
	BinaryOp(Expression left, std::string op, Expression right) :
		left(left), op(op), right(right) {}
};
