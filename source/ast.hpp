#pragma once

#include <vector>
#include <memory>

#include "CodeGeneration/Generator.hpp"

typedef double NUMBER_T;


class Parameter {
public:
	virtual void accept(Generator * generator) {}
};

class Statement {
public:
	virtual void accept(Generator * generator) {}
};

class Expression {
public:
	virtual void accept(Generator * generator) {}
};

class Block
{
public:
	std::vector<std::shared_ptr<Statement>> statements;
	bool topLevel = false;

	Block(std::vector<std::shared_ptr<Statement>> statements) :
		statements(statements) {}

	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};

class Function
{
public:
	std::string name;
	std::vector<std::shared_ptr<Parameter>> parameters;
	Block block;

	Function(std::string name, std::vector<std::shared_ptr<Parameter>> parameters, Block block) :
		name(name), parameters(parameters), block(block)
	{
		block.topLevel = true;
	}
	
	void accept(Generator * generator)
	{
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
		generator->generate(*this);
	}
};


class Lambda : public Statement
{
public:
	std::string name;
	std::vector<IdParameter> parameters;
	Expression expression;
	
	Lambda(std::string name, std::vector<IdParameter> parameters, Expression expression) :
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
	std::shared_ptr<Expression> expression;
	
	ExpressionStatement(std::shared_ptr<Expression> expression) :
		expression(expression) {}
	
	void accept(Generator * generator)
	{
		expression->accept(generator);
	}
};

class Number : public Expression
{
public:
	NUMBER_T value;
	
	Number(NUMBER_T value) :
		value(value) {}
		
	void accept(Generator * generator)
	{
		generator->generate(*this);
	}

};

class Id : public Expression
{
public:
	std::string name;
	
	Id(std::string name) :
		name(name) {}
	
	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
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
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;
	std::string op;
	
	BinaryOp(std::shared_ptr<Expression> left, std::string op, std::shared_ptr<Expression> right) :
		left(left), op(op), right(right) {}

	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};
