#include "Context.hpp"
#include "ast.hpp"

Function * FunctionBundle::getBase()
{
	if (sym == "main") {
		return nullptr;
	}
	
	for (Function * function : impls) {
		if (!function->isSpecification()) {
			return function;
		}
	}
	
	return nullptr;
}

void FunctionBundle::addFunction(Function * function)
{
	assert(function->parameters.size() == argCount);

	if (!function->isSpecification() && getBase() != nullptr) {
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
	toPrelude();
	
	for(auto it = functions.begin(); it != functions.end(); it++) {
		FunctionBundle& bundle = it->second;
		
		if (bundle.isMain()) {
			continue;
		}
		
		Function* base = bundle.getBase();
		
		if (base == nullptr) {
			std::cout << "Function with sym: " << bundle.getSym() << ", has no base." << std::endl;
			exit(1);
		}

		// Render header
		out() << "inline " << base->renderHeader() << std::endl << "{" << std::endl;


		// If length of implementations is greater than 1, then the symbol
		// has specializations, since there can only be (and must be) one base
		// per symbol. This is controlled when anexing them to its bundle.
		
		if (bundle.getImpls().size() > 1) {
			bool firstSpecialization = true;
				
			for (Function* function : bundle.getImpls()) {
				// Ignore base
				if (function == base) {
					continue;
				}

				if (!firstSpecialization) {
					out() << "else ";
				}
				
				firstSpecialization = false; // do not get confused :D
				
				out() 	<< "    if (" << function->getCondition(base) << ") {" << std::endl
						<< "        return " << function->renderCall() << std::endl << "    } ";
			}
			
			out() << "else {" << std::endl << "    ";
		}
		
		out() 	<< "    return " << base->renderCall() << std::endl;
		
		if (bundle.getImpls().size() > 1) {
			out() << "    }" << std::endl;
		}
		
		out() << "}" << std::endl << std::endl;
	}
}