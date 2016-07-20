#include "Generator.hpp"

#include "../ast.hpp"

void Generator::generate(std::vector<Function> functions)
{
	for (auto i : functions) {
		i.accept(this);
	}
}

void Generator::generate(Function & function)
{
	if (function.name == "main") {
		std::cout << "int ";
	} else {
		std::cout << "double ";
	}
	
	std::cout << function.name;
	std::cout << "(";

	for (int i = 0; i < function.parameters.size(); i++) {
		if (i > 0) {
			std::cout << ", ";
		}
		
		std::cout << "double ";
		function.parameters[i]->accept(this);
	}
	
	std::cout << ")" << std::endl << "{" << std::endl;
	
	function.block.accept(this);
	
	std::cout << "}" << std::endl;
}

void Generator::generate(IdParameter & parameter)
{
	std::cout << parameter.name;
}

void Generator::generate(NumberParameter & parameter)
{
	
}

void Generator::generate(EpsilonParameter & parameter)
{
	
}

void Generator::generate(Block & block)
{
	for (int i = 0; i < block.statements.size(); i++) {
		std::cout << "    ";
		
		if (i == block.statements.size() - 1) {
			std::cout << "return ";
		}
		
		block.statements[i]->accept(this);
		std::cout << ";" << std::endl;
	}
}

void Generator::generate(Id & id)
{
	std::cout << id.name;
}

void Generator::generate(BinaryOp & binaryOp)
{
	if (binaryOp.op == "^") {
		std::cout << "pow(";
		binaryOp.left->accept(this);
		std::cout << ", ";
	
	} else {
		std::cout << "(";
		binaryOp.left->accept(this);
		std::cout << " " << binaryOp.op << " ";
	}

	binaryOp.right->accept(this);
	std::cout << ")";
}

void Generator::generate(Number & number)
{
	std::cout << number.value;
}