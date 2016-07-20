#include <iostream>
#include <vector>

#include "ast.hpp"

extern int yyparse();

extern std::vector<Function*> functions;

int main(int argc, char **argv)
{
	yyparse();

	return 0;
}
