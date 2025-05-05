#include "../bc.hh"
#include "ast.hh"
#include <pch.hh>

using namespace bc;
using namespace llvm;

void AstNumber::print(int indent) const {
	printIndent(indent);
	std::cout << "AstNumber" << std::endl;
	printIndent(indent + 1);
	std::cout << "value: " << value << std::endl;
}

bool AstNumber::compile(BC* bc) {
	Value* v = ConstantInt::get(bc->builder.getInt32Ty(), value);
	bc->values.push(v);
	return true;
}
