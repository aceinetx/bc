#include "../bc.hh"
#include "ast.hh"
#include <pch.hh>

using namespace bc;
using namespace llvm;

void AstVarAssign::print(int indent) const {
	printIndent(indent);
	std::cout << "AstVarAssign" << std::endl;
	printIndent(indent + 1);
	std::cout << "name: " << name << std::endl;
	printIndent(indent + 1);
	std::cout << "expr:" << std::endl;
	expr->print(indent + 2);
}

AstVarAssign::~AstVarAssign() {
	delete expr;
}

bool AstVarAssign::compile(BC* bc) {
	Variable* var = bc->getVariable(name);
	if (!var) {
		bc->compile_error = fmt::format("undefined variable: {}", name);
		return false;
	}

	if (!expr->compile(bc))
		return false;

	Value* expr_value = bc->values.top();
	bc->values.pop();

	bc->builder.CreateStore(expr_value, var->value);

	return true;
}
