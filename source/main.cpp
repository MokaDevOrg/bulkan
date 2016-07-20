#include <iostream>
#include <vector>

#include "CodeGeneration/Generator.hpp"
#include "ast.hpp"

extern int yyparse();

std::vector<Function> functions;

int main(int argc, char **argv)
{
	yyparse();

	Generator generator;
	generator.generate(functions);

	generator.context.toPrelude();
	std::cout << generator.context.out().str();
	
	generator.context.toBody();
	std::cout << generator.context.out().str();
	
	return 0;
}
