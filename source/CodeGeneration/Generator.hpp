#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "../ast.hpp"

class Generator
{
private:
	

public:
	void generate(std::vector<Function> functions);

	void generate(Function & function);
};
