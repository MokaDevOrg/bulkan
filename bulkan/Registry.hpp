#pragma once

#include <map>

class Registry
{
private:
	std::map<std::string, std::string*> table;

public:
	~Registry()
	{
		for(auto iterator = table.begin(); iterator != table.end(); iterator++) {
			delete iterator->second;
		}
	}
	
	std::string * get(const char * name, int yyleng)
	{
		auto str = std::string(name, yyleng);
		
		if (table.find(name) == table.end()) {
			table[str] = new std::string(name);
		}
		
		return table[name];
	}
} registry;
