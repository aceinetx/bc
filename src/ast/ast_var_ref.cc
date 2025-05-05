#include "../bc.hh"
#include "ast.hh"
#include <pch.hh>

using namespace bc;

void AstVarRef::print(int indent) const {
	printIndent(indent);
	std::cout << name << std::endl;
}

bool AstVarRef::compile(BC* bc) {
	bc->values.push(bc->builder.CreateLoad(bc->builder.getInt32Ty(), bc->getVariable(name)->value));
	return true;
}
