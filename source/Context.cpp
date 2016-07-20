#include "Context.hpp"
#include "ast.hpp"


void Context::addFunction(Function * function)
{
	if (functions.find(function->name) != functions.end()) {
		std::cerr << "Function already defined: " << function->name << std::endl;
		exit(1);
	}
	
	functions[function->name] = function;
	lastFunction = function;
}
