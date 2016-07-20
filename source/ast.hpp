#pragma once

#include <vector>

#include "CodeGeneration/Generator.hpp"

typedef double NUMBER_T;


class Parameter {
public:
	virtual void accept(Generator * generator)
	{
		std::cout << "GG" << std::endl;
	}
};

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
	
	void accept(Generator * generator) {
		generator->generate(*this);
	}
};



class IdParameter : public Parameter
{
public:
	std::string name;
	
	IdParameter(std::string name) :
		name(name) {}

    void accept(Generator * generator)
	{
		std::cout << "GG";
		generator->generate(*this);
	}
};

class NumberParameter : public Parameter
{
public:
	NUMBER_T value;
	
	NumberParameter(NUMBER_T value):
		value(value) {}
	
	void accept(Generator * generator)
	{
		std::cout << "GG";
		generator->generate(*this);
	}
};

class EpsilonParameter : public NumberParameter
{
public:
	NUMBER_T epsilon;
	
	EpsilonParameter(NUMBER_T value, NUMBER_T epsilon) :
		NumberParameter(value), epsilon(epsilon) {}

	void accept(Generator * generator)
	{
		std::cout << "GG";
		generator->generate(*this);
	}
};


class Lambda : public Statement
{
public:
	std::string name;
	std::vector<Parameter> parameters;
	Expression expression;
	
	Lambda(std::string name, std::vector<Parameter> parameters, Expression expression) :
		name(name), parameters(parameters), expression(expression) {}
};

class VariableDecl : public Statement
{
public:
	std::string name;
	Expression expression;
	
	VariableDecl(std::string name, Expression expression) :
		name(name), expression(expression) {}
};

class Assignment : public Statement
{
public:
	std::string name;
	Expression expression;
	
	Assignment(std::string name, Expression expression) :
		name(name), expression(expression) {}
};


class ExpressionStatement : public Statement
{
public:
	Expression expression;
	
	ExpressionStatement(Expression expression) :
		expression(expression) {}
};

class Number : public Expression
{
public:
	NUMBER_T value;
	
	Number(NUMBER_T value) :
		value(value) {}
};

class Id : public Expression
{
public:
	std::string name;
	
	Id(std::string name) :
		name(name) {}
};

class FunctionCall : public Expression
{
public:
	std::string value;
	std::vector<Expression> expressionList;
	
	FunctionCall(std::string value, std::vector<Expression> expressionList) :
		value(value), expressionList(expressionList) {}
};

class BinaryOp : public Expression
{
public:
	Expression left;
	Expression right;
	std::string op;
	
	BinaryOp(Expression left, std::string op, Expression right) :
		left(left), op(op), right(right) {}
};
