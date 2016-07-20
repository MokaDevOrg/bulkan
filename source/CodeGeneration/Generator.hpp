#pragma once

#include <vector>
#include <iostream>
#include <string>


class Function;
class IdParameter;
class NumberParameter;
class EpsilonParameter;


class Generator
{
private:
	

public:
	void generate(std::vector<Function> functions);

	void generate(Function & function);

	void generate(IdParameter & parameter);
	
	void generate(NumberParameter & parameter);
	
	void generate(EpsilonParameter & parameter);
};
