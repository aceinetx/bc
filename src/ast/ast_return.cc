#include "../bc.hh"
#include "ast.hh"
#include <pch.hh>

using namespace bc;

void AstReturn::print(int indent) const {
	printIndent(indent);
	std::cout << "AstReturn" << std::endl;
	if (expr) {
		expr->print(indent + 1);
	}
}

AstReturn::~AstReturn() {
	delete expr;
}

bool AstReturn::compile(BC* bc) {
	if (!expr->compile(bc))
		return false;

	bc->builder.CreateRet(bc->values.top());
	bc->values.pop();

	return true;
}
