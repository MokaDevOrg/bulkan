#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <sstream>

#include "codegen/Generator.hpp"

typedef double NUMBER_T;

extern size_t yyline;

class Expression;
class Statement;

typedef std::vector<std::shared_ptr<Expression>> ExpressionList;
typedef std::vector<std::shared_ptr<Statement>> StatementList;


class Parameter {
public:
	size_t line = yyline;
	
	virtual bool isId() const
	{
		return false;
	}
	
	virtual std::string getCondition(std::string id)
	{
		assert(!isId());
		return "";
	}
	
	virtual std::string render()
	{
		return "";
	}
	
	virtual void accept(Generator * generator) {}
};

class Statement {
public:
	size_t line = yyline;
	
	virtual void accept(Generator * generator) {}
};

class Expression {
public:
	size_t line = yyline;
	
	virtual void accept(Generator * generator) {}
};

class ListId
{
public:
	std::string name;
	ExpressionList expressionList;

	ListId(std::string name, ExpressionList expressionList) : 
		name(name), expressionList(expressionList) {}
};

class Block
{
public:
	std::vector<std::shared_ptr<Statement>> statements;
	bool topLevel = false;
	bool inMain = false;

	size_t line = yyline;

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

	std::string render()
	{
		return name;
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
	
	std::string render()
	{
		return std::to_string(value);
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

	std::string render()
	{
		std::stringstream ss;
		ss << value << " +- " << epsilon;
		return ss.str();
	}

	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};

class ListParameter : public Parameter
{
public:
	std::shared_ptr<ListId> id;

	ListParameter(std::shared_ptr<ListId> id) : id(id) {}

	std::string getCondition(std::string id)
	{
		return "";
	}

	std::string render()
	{
		return "";
	}

	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};

class Function
{
private:
	bool isSpecificationCache;
	bool isSpecificationCached;
	
	std::string realName;
	bool realNameSet;
	
public:
	std::string name;
	std::vector<std::shared_ptr<Parameter>> parameters;
	Block block;

	Function(std::string name, std::vector<std::shared_ptr<Parameter>> parameters, Block block) :
		isSpecificationCached(false), realNameSet(false), name(name), parameters(parameters), block(block)
	{
		block.topLevel = true;
	}
	
	~Function()
	{
	}
	
	bool isSpecification()
	{
		if (isSpecificationCached) {
			return isSpecificationCache;
		}
		
		for (auto& parameter : parameters) {
			if (!parameter->isId()) {
				isSpecificationCached = true;
				break;
			}
		}
		
		isSpecificationCache = isSpecificationCached;
		isSpecificationCached = true;
		
		return isSpecificationCache;
	}
	
	std::string getCondition(Function* base)
	{
		assert(!base->isSpecification());
		assert(isSpecification());

		std::stringstream ss;
		bool condBefore = false;
		
		for (int i = 0; i < parameters.size(); i++) {
			if (!parameters[i]->isId()) {
				if (condBefore) {
					ss << " && ";
				}
				
				IdParameter * param = static_cast<IdParameter*>(base->parameters[i].get());
				ss << parameters[i]->getCondition(param->name);
				
				condBefore = true;
			}
		}
		
		return ss.str();
	}
	
	void setRealName(std::string realName)
	{
		assert(!realNameSet);
		realNameSet = true;
		this->realName = realName;
	}
	
	std::string getRealName() const
	{
		return realName;
	}
	
	std::string renderHeader(bool useRealName = false)
	{
		// For now this should be called on a base function
		assert(!isSpecification());
		std::stringstream ss;
		
		if (name == "main") {
			ss << "int ";
			block.inMain = true; // ?
		} else {
			ss << "double ";
		}

		if (useRealName) {
			ss << realName;
		} else {
			ss << name;
		}
		
		ss << "(";

		for (int i = 0; i < parameters.size(); i++) {
			if (i > 0) {
				ss << ", ";
			}

			ss << "double ";
			
			IdParameter* param = static_cast<IdParameter*>(parameters[i].get());
			ss << param->name;
		}
		
		ss << ")";
		return ss.str();
	}
	
	/**
	* This method renders a call to this function given its
	* parameters. Note that this is not the wanted behaviour
	* in some cases, so we can pass base function reference
	* in order to use its parameters. Also, if base reference
	* is given, then parameters in same position as specialized
	* ones are ignored.
	*
	* ATTENTION: The latter means ranges are also ignored. A
	* proposal of binding them to a name has been submited.
	*/
	std::string renderCall(Function* base = nullptr)
	{
		std::vector<std::shared_ptr<Parameter>>&
			parameters = (base == nullptr) ? this->parameters : base->parameters;

		std::stringstream ss;
		ss << realName << "(";
		
		for (int i = 0; i < parameters.size(); i++) {
			if (!this->parameters[i]->isId()) {
				continue;
			}

			if (i > 0) {
				ss << ", ";
			}
			
			ss << parameters[i]->render();
		}

		ss << ");";
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

class ListDecl : public Statement
{
public:
	std::shared_ptr<ListId> id;
	std::shared_ptr<Expression> expression;
	
	ListDecl(std::shared_ptr<ListId> id, std::shared_ptr<Expression> expression) :
		id(id), expression(expression) {}
		
	ListDecl(std::shared_ptr<ListId> id) : ListDecl(id, nullptr) {}

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
	std::string string;

	Log(std::shared_ptr<Expression> expression) :
		expression(expression) {}
	
	Log(std::string string) :
		string(string) {}

	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};

class Assert : public Statement
{
public:
	std::shared_ptr<Expression> expression;
	std::string errorMessage;

	Assert(std::shared_ptr<Expression> expression, std::string errorMessage) :
		expression(expression), errorMessage(errorMessage) {}

	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};

class For : public Statement
{
public:
	std::string variable;
	std::shared_ptr<Expression> lower;
	std::shared_ptr<Expression> upper;
	Block block;
	bool inclusive;

	For(std::string variable, std::shared_ptr<Expression> lower, std::shared_ptr<Expression> upper,
			Block block, bool inclusive) :
		variable(variable), lower(lower), upper(upper), block(block), inclusive(inclusive) {}

	void accept(Generator * generator)
	{
		generator->generate(*this);
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

class ListElement : public Expression
{
public:
	std::shared_ptr<ListId> id;
	
	ListElement(std::shared_ptr<ListId> id) : id(id) {}

	void accept(Generator * generator)
	{
		generator->generate(*this);
	}
};