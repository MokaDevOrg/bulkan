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
	std::cout << function.name << std::endl;
}
