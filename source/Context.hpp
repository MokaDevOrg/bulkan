#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <vector>


class Function;


class Scope
{
private:
	std::vector<std::string> symbols;
	std::vector<std::string> lambdas;
	bool parentLookUp;

public:
	std::shared_ptr<Scope> parent;
	
	Scope(bool parentLookUp) :
		parentLookUp(parentLookUp) {}

	bool hasSymbol(std::string name)
	{
		for (auto i : symbols) {
			if (i == name) {
				return true;
			}
		}
		
		if (parentLookUp && parent) {
			return parent->hasSymbol(name);
		}
		
		return false;
	}
	
	
	bool hasLambda(std::string name)
	{
		for (auto i : lambdas) {
			if (i == name) {
				return true;
			}
		}

		if (parentLookUp && parent) {
			return parent->hasLambda(name);
		}

		return false;
	}
	
	void addSymbol(std::string name, bool isLambda)
	{
		if (hasSymbol(name)) {
			std::cerr << "Symbol already defined: " << name << std::endl;
			exit(1);
		}
		
		symbols.push_back(name);
		
		if (isLambda) {
			lambdas.push_back(name);
		}
	}
	
	void assertSymbol(std::string name)
	{
		if (!hasSymbol(name)) {
			std::cerr << "Symbol not defined: " << name << std::endl;
			exit(1);
		}
	}

	void assertLambda(std::string name)
	{
		if (!hasLambda(name)) {
			std::cerr << "Lambda not defined: " << name << std::endl;
			exit(1);
		}
	}
};



class Context
{
private:
	std::map<std::string, Function*> functions;
	
	Function * lastFunction = nullptr;

	std::ostringstream prelude;
	std::ostringstream body;

	std::ostringstream * current;
	
	std::shared_ptr<Scope> scope;

public:
	Context()
	{
		toBody();
	}
	
	void addFunction(Function * function);
	
	bool hasFunction(std::string name)
	{
		if (functions.find(name) == functions.end()) {
			return false;
		}

		return true;
	}

	Function & getFunction(std::string name)
	{
		return *functions[name];
	}
	
	Function & getFunction()
	{
		return *lastFunction;
	}
	
	void toPrelude()
	{
		current = &prelude;
	}
	
	void toBody()
	{
		current = &body;
	}
	
	std::ostringstream & out()
	{
		return *current;
	}

	std::shared_ptr<Scope> createScope(bool parentLookUp)
	{		
		auto newScope = new Scope(parentLookUp);
		newScope->parent = scope;
		scope = std::shared_ptr<Scope>(newScope);

		return scope;
	}
	
	std::shared_ptr<Scope> getScope()
	{
		return scope;
	}
	
	void popScope()
	{
		auto parent = scope->parent;
		scope->parent.reset();
		scope = parent;
	}
};
