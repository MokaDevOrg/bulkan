#include "Generator.hpp"

#include "../ast.hpp"
#include "../Context.hpp"


void Generator::generate(std::vector<Function> functions)
{
	for (auto i : functions) {
		i.accept(this);
	}
}

void renderFunctionHeader(Generator * generator, Context & context, Function & function)
{
	if (function.name == "main") {
		context.out() << "int ";
		function.block.inMain = true;
	} else {
		context.out() << "double ";
	}

	context.out() << function.name;
	context.out() << "(";

	for (int i = 0; i < function.parameters.size(); i++) {
		if (i > 0) {
			context.out() << ", ";
		}

		context.out() << "double ";
		function.parameters[i]->accept(generator);
	}
	
	context.out() << ")";
}

void Generator::generate(Function & function)
{
	context.addFunction(&function);
	auto scope = context.createScope(false);

	// create header code.
	context.toPrelude();
	renderFunctionHeader(this, context, function);
	context.out() << ";" << std::endl << std::endl;

	// create body.
	context.toBody();
	renderFunctionHeader(this, context, function);

	context.out() << std::endl << "{" << std::endl;

	function.block.accept(this);

	context.out() << "}" << std::endl << std::endl;

	context.popScope();
}

void Generator::generate(IdParameter & parameter)
{
	context.getScope()->addSymbol(parameter.name, false, true);
	context.out() << parameter.name;
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
		context.out() << "    ";

		if (block.topLevel && !block.inMain && i == block.statements.size() - 1) {
			context.out() << "return ";
		}

		block.statements[i]->accept(this);
		context.out() << ";" << std::endl;
	}
	
	if (block.inMain) {
		context.out() << "    return 0;" << std::endl;
	}
}

void Generator::generate(Id & id)
{
	context.getScope()->assertSymbol(id.name);
	context.out() << id.name;
}

void Generator::generate(BinaryOp & binaryOp)
{
	if (binaryOp.op == "^") {
		context.out() << "pow(";
		binaryOp.left->accept(this);
		context.out() << ", ";
	} else {
		context.out() << "(";
		binaryOp.left->accept(this);
		context.out() << " " << binaryOp.op << " ";
	}

	binaryOp.right->accept(this);
	context.out() << ")";
}

void Generator::generate(Number & number)
{
	context.out() << number.value;
}

void Generator::generate(Lambda & lambda)
{
	auto functionScope = context.getScope();
	functionScope->addSymbol(lambda.name, true);
	
	context.createScope(false);
	context.toPrelude();
	
	auto function = context.getFunction();

	context.out() << "double " << function.name << "__" << lambda.name << "(";

	// parameters.
	for (int i = 0; i < lambda.parameters.size(); i++) {
		if (i > 0) {
			context.out() << ", ";
		}
		
		context.out() << "double ";
		lambda.parameters[i].accept(this);
	}

	context.out() << ")" << std::endl << "{" << std::endl;

	// return expression.
	context.out() << "    return ";
	lambda.expression->accept(this);
	context.out() << ";" << std::endl;

	context.out() << "}" << std::endl << std::endl;

	context.toBody();
	context.popScope();
	
	context.out() << "// lambda defined: " << lambda.name;
}

void Generator::generate(FunctionCall & functionCall)
{
	// TODO: search lambda and then functions.
	if (context.hasFunction(functionCall.name)) {
		context.out() << functionCall.name << "(";
	} else {
		context.getScope()->assertLambda(functionCall.name);

		auto functionName = context.getFunction().name;
		context.out() << functionName << "__" << functionCall.name << "(";
	}
	
	for (int i = 0; i < functionCall.expressionList.size(); i++) {
		if (i > 0) {
			context.out() << ", ";
		}
		
		functionCall.expressionList[i]->accept(this);
	}
	
	context.out() << ")";
}

void Generator::generate(Assignment & assignment)
{
	context.getScope()->assertSymbol(assignment.name);
	
	context.out() << assignment.name << " = ";
	assignment.expression->accept(this);
}

void Generator::generate(VariableDecl & varDecl)
{	
	context.getScope()->addSymbol(varDecl.name, false);
	
	context.out() << "double ";
	context.out() << varDecl.name << " = ";
	varDecl.expression->accept(this);
}

void Generator::generate(Log & log)
{
	
}
