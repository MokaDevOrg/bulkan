#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "../Context.hpp"

class Context;


class Function;
class IdParameter;
class NumberParameter;
class EpsilonParameter;
class Block;
class Id;
class BinaryOp;
class Number;
class Lambda;
class FunctionCall;


class Generator
{
public:
	Context context;
	
	void generate(std::vector<Function> functions);

	void generate(Function & function);

	void generate(IdParameter & parameter);
	
	void generate(NumberParameter & parameter);
	
	void generate(EpsilonParameter & parameter);

	void generate(Block & block);
	
	void generate(Id & id);
	
	void generate(BinaryOp & binaryOp);
	
	void generate(Number & number);
	
	void generate(Lambda & lambda);
	
	void generate(FunctionCall & functionCall);
};
