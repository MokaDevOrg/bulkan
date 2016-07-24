#include <iostream>
#include <vector>

#include "CodeGeneration/Generator.hpp"
#include "ast.hpp"

extern int yyparse();

std::vector<Function*> functions;

int main(int argc, char **argv)
{
	yyparse();

	Generator generator;
	generator.generate(functions);
	
	std::cout << "#include <math.h>" << std::endl;
	std::cout << "#include <stdio.h>" << std::endl;
	std::cout << std::endl;

	generator.context.toPrelude();
	std::cout << "// prelude." << std::endl;
	std::cout << generator.context.out().str();

	std::cout << std::endl;

	std::cout << "// body." << std::endl;
	generator.context.toBody();
	std::cout << generator.context.out().str();
	
	// Function dummies
	std::cout << "generateFunctionDummies()" << std::endl;
	generator.context.generateFunctionDummies();
	
	return 0;
}
