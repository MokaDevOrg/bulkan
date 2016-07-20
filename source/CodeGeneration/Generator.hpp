#pragma once

#include <vector>
#include <iostream>
#include <string>

class Function;

class Generator
{
private:
	

public:
	void generate(std::vector<Function> functions);

	void generate(Function & function);
};
