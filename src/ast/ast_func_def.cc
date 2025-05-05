#include "../bc.hh"
#include "ast.hh"
#include <llvm/IR/BasicBlock.h>
#include <pch.hh>

using namespace bc;
using namespace llvm;

void AstFuncDef::print(int indent) const {
	printIndent(indent);
	std::cout << "AstFuncDef" << std::endl;
	printIndent(indent + 1);
	std::cout << "name: " << name << std::endl;
	for (const auto& stmt : children) {
		stmt->print(indent + 1);
	}
}

bool AstFuncDef::compile(BC* bc) {
	auto type = FunctionType::get(bc->builder.getInt32Ty(), {}, false);
	auto func = Function::Create(type, Function::ExternalLinkage, name, bc->fmodule);
	auto block = BasicBlock::Create(bc->context, "_" + name, func);

	bc->builder.SetInsertPoint(block);

	bc->scopes.push_back({});

	if (!AstNode::compile(bc))
		return false;

	if (!block->getTerminator()) {
		bc->builder.CreateRet(ConstantInt::get(bc->builder.getInt32Ty(), 0));
	}

	return true;
}
