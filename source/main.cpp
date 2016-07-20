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

	return 0;
}
