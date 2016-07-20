#include "Context.hpp"
#include "ast.hpp"

Function * FunctionBundle::getBase()
{
	if (sym == "main") {
		return nullptr;
	}
	
	for (Function * function : impls) {
		std::cout << "checking on " << function->name << std::endl;
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
	ss << function->name;
	
	if (function->name != "main") {
		ss << "_" << impls.size();
	}
	
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
	for(auto it = functions.begin(); it != functions.end(); it++) {
		//FunctionBundle & functionBundle = it->second;

		std::cout << "sym: " << it->second.getSym() << std::endl;
		for (Function * fn : it->second.getImpls()) {
			std::cout << fn->getRealName() << std::endl;
		}
		
		//Function * base = functionBundle.getBase();
		//std::cout << functionBundle.getSym() << std::endl;
		//std::cout << base->name << std::endl;
	}
}