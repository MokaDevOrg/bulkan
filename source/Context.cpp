#include "Context.hpp"
#include "ast.hpp"

Function * FunctionBundle::getBase()
{
	for (Function * function : impls) {
		if (function->isSpecification()) {
			return function;
		}
	}
	
	return nullptr;
}

void FunctionBundle::addFunction(Function * function)
{
	assert(function->parameters.size() == argCount);
	
	if (function->isSpecification() && getBase() != nullptr) {
		std::cout << "There cannot be 2 base functions of the same." << std::endl;
		exit(1);
	}
	
	std::stringstream ss;
	ss << function->name << "_" << impls.size();		
	function->setRealName(ss.str());

	impls.push_back(function);
}	

void Context::addFunction(Function * function)
{
	if (functions.find(function->name) == functions.end()) {
		functions[function->name] = FunctionBundle(function->name, function->parameters.size());
	}
	
	functions[function->name].addFunction(function);
	lastFunction = function;
}

void Context::generateFunctionDummies()
{
	typedef std::map<std::string, FunctionBundle>::iterator it_type;
	for(it_type it = functions.begin(); it != functions.end(); it++) {
		FunctionBundle & functionBundle = it->second;

	}
}