#include "Context.hpp"
#include "ast.hpp"


void Context::addFunction(Function * function)
{
	if (functions.find(function->name) == functions.end()) {
		functions[function->name] = FunctionBundle(function->name);
	}
	
	functions[function->name].addFunction(function);
	lastFunction = function;
}
