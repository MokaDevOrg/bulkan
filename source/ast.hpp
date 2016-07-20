#pragma once

#include <vector>

typedef double NUMBER_T;

struct Parameter;
struct Statement;
struct Block;

struct Function
{
	std::string name;
	std::vector<Parameter> parameters;
	Block block;
	
	Function(std::string name, std::vector<Parameter> paramaters, Block block) :
		name(name), parameters(parameters), block(block) {}
};

struct Parameter {};

struct IdParameter : Parameter
{
	std::string name;
	
	IdParameter(std::string name) :
		name(name) {}
};

struct NumberParameter : Parameter
{
	NUMBER_T value;
	
	NumberParameter(NUMBER_T value):
		value(value) {}
};

struct EpsilonParameter : NumberParameter
{
	NUMBER_T epsilon;
	
	EpsilonParameter(NUMBER_T value, NUMBER_T epsilon) :
		NumberParameter(value), epsilon(epsilon) {}
};


struct Block
{
	std::vector<Statement> statements;
	
	Block(std::vector<Statement> statements) :
		statements(statements) {}
};


struct Statement {};
struct Expression {};


struct Lambda : Statement
{
	
};

struct VariableDecl : Statement
{
};

struct Assignment : Statement
{
};


struct ExpressionStatement : Statement
{
};

struct Number : Expression
{
};

struct Id : Expression
{
};

struct FunctionCall : Expression
{
};

struct BinaryOp : Expression
{
};

