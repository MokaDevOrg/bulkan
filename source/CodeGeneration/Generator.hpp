#pragma once

#include <vector>
#include <iostream>
#include <string>


class Function;
class IdParameter;
class NumberParameter;
class EpsilonParameter;
class Block;
class Id;
class BinaryOp;
class Number;

class Generator
{
private:
	

public:
	void generate(std::vector<Function> functions);

	void generate(Function & function);

	void generate(IdParameter & parameter);
	
	void generate(NumberParameter & parameter);
	
	void generate(EpsilonParameter & parameter);

	void generate(Block & block);
	
	void generate(Id & id);
	
	void generate(BinaryOp & binaryOp);
	
	void generate(Number & number);
};
