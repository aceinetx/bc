#include "../bc.hh"
#include "ast.hh"
#include <pch.hh>

using namespace bc;
using namespace llvm;

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

bool AstFuncCall::compile(BC* bc) {
	auto func = bc->fmodule.getFunction(name);
	if (!func) {
		bc->compile_error = fmt::format("undefined function: {}", name);
		return false;
	}

	std::vector<Value*> arg_values = {};
	for (auto arg : args) {
		arg->compile(bc);
		arg_values.push_back(bc->values.top());
		bc->values.pop();
	}

	bc->values.push(bc->builder.CreateCall(func, arg_values));

	return true;
}
