#include "ast.hh"
#include <pch.hh>

using namespace bc;

void AstFuncCall::print(int indent) const {
	printIndent(indent);
	std::cout << "AstFuncCall" << std::endl;
	printIndent(indent + 1);
	std::cout << "name: " << name << std::endl;
	for (AstNode* arg : args) {
		arg->print(indent + 1);
	}
}

AstFuncCall::~AstFuncCall() {
	for (AstNode* arg : args) {
		delete arg;
	}
}
