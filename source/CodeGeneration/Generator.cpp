#include "Generator.hpp"

#include "../ast.hpp"

void Generator::generate(std::vector<Function> functions)
{
	for (auto i : functions) {
		i.accept(this);
	}
}

void Generator::generate(Function & function)
{
	if (function.name == "main") {
		std::cout << "int ";
	} else {
		std::cout << "double ";
	}
	
	std::cout << function.name;
	std::cout << "(";

	for (int i = 0; i < function.parameters.size(); i++) {
		if (i > 0) {
			std::cout << ", ";
		}
		
		std::cout << "double ";
		function.parameters[i]->accept(this);
	}
	
	std::cout << ")" << std::endl;
}

void Generator::generate(IdParameter & parameter)
{
	std::cout << parameter.name;
}

void Generator::generate(NumberParameter & parameter)
{
	
}

void Generator::generate(EpsilonParameter & parameter)
{
	
}
