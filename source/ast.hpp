#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <sstream>

#include "CodeGeneration/Generator.hpp"

typedef double NUMBER_T;


class Parameter {
public:
	virtual bool isId()
	{
		return false;
	}
	
	virtual std::string getCondition(std::string id)
	{
		assert(!isId());
		return "";
	}
	
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
	bool inMain = false;

	Block(std::vector<std::shared_ptr<Statement>> statements, bool topLevel) :
		statements(statements), topLevel(topLevel) {}

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

	bool isId() const
	{
		return true;
	}

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
	
	std::string getCondition(std::string id)
	{
		std::stringstream ss;
		ss << id << " == " << value;
		return ss.str();
	}
	
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

	std::string getCondition(std::string id)
	{
		std::stringstream ss;
		ss << id << ">=" << (value - epsilon) << " && " << id << " <= " << (value + epsilon);
		return ss.str();
	}

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
	
	bool isSpecification() const
	{
		for (auto& parameter : parameters) {
			if (!parameter->isId()) {
				return true;
			}
		}
		
		return false;
	}
	
	std::string getCondition(const Function& base)
	{
		assert(!base.isSpecification());
		assert(this != &base);

		std::stringstream ss;
		
		for (int i = 0; i < parameters.size(); i++) {
			if (!parameters[i]->isId()) {
				IdParameter * param = static_cast<IdParameter*>(base.parameters[i].get());
				ss << parameters[i]->getCondition(param->name);
				
				if (i != parameters.size() - 1) {
					ss << " && ";
				}
			}
		}
		
		return ss.str();
	}
	
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
	std::shared_ptr<Expression> expression;
	
	Lambda(std::string name, std::vector<IdParameter> parameters, std::shared_ptr<Expression> expression) :
		name(name), parameters(parameters), expression(expression) {}
	
	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};

class VariableDecl : public Statement
{
public:
	std::string name;
	std::shared_ptr<Expression> expression;
	
	VariableDecl(std::string name, std::shared_ptr<Expression> expression) :
		name(name), expression(expression) {}
	
	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};

class Assignment : public Statement
{
public:
	std::string name;
	std::shared_ptr<Expression> expression;
	
	Assignment(std::string name, std::shared_ptr<Expression> expression) :
		name(name), expression(expression) {}
	
	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
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

class Log : public Statement
{
public:
	std::shared_ptr<Expression> expression;
	
	Log(std::shared_ptr<Expression> expression) :
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
	std::string name;
	std::vector<std::shared_ptr<Expression>> expressionList;
	
	FunctionCall(std::string name, std::vector<std::shared_ptr<Expression>> expressionList) :
		name(name), expressionList(expressionList) {}
	
	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
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
