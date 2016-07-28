#include <iostream>
#include <vector>
#include <fstream>

#include "bulkan/codegen/Generator.hpp"
#include "bulkan/ast.hpp"

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

	// Function dummies
	generator.context.generateFunctionDummies();

	generator.context.toPrelude();
	std::cout << "// prelude." << std::endl;
	std::cout << generator.context.out().str();

	std::cout << std::endl;

	std::cout << "// body." << std::endl;
	generator.context.toBody();
	std::cout << generator.context.out().str();

	std::ofstream prelude;
	std::ofstream body;
	
	// Write to files
    prelude.open("output/output_prelude.h");
	body.open("output/output_body.c");

	generator.context.toPrelude();
	prelude << "#include <math.h>" << std::endl;
	prelude << "#include <stdio.h>" << std::endl;
	prelude << "#include <stdlib.h>" << std::endl;
	prelude << std::endl;
	prelude << generator.context.out().str();

	generator.context.toBody();
	body << "#include \"output_prelude.h\"" << std::endl;
	body << std::endl;
	body << generator.context.out().str();

	prelude.close();
	body.close();

	return 0;
}
