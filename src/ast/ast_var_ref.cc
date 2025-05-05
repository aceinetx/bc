#include "../bc.hh"
#include "ast.hh"
#include <pch.hh>

using namespace bc;

void AstVarRef::print(int indent) const {
	printIndent(indent);
	std::cout << name << std::endl;
}

bool AstVarRef::compile(BC* bc) {
	Variable* var = bc->getVariable(name);
	if (!var) {
		bc->compile_error = fmt::format("undefined variable: {}", name);
		return false;
	}

	bc->values.push(bc->builder.CreateLoad(bc->builder.getInt32Ty(), var->value));
	return true;
}
