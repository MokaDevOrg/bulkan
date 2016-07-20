#include <iostream>
#include <vector>

#include "ast.hpp"

extern int yyparse();

std::vector<Function> functions;

int main(int argc, char **argv)
{
	yyparse();
	
	for (auto i : functions) {
		
	}

	return 0;
}
