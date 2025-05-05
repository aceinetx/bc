#include "../bc.hh"
#include "ast.hh"
#include <pch.hh>

using namespace bc;

void AstVarDeclare::print(int indent) const {
	printIndent(indent);
	std::cout << "AstVarDeclare" << std::endl;
	printIndent(indent + 1);
	std::cout << "names: ";
	for (std::string name : names) {
		std::cout << name << " ";
	}
	std::cout << std::endl;
}

bool AstVarDeclare::compile(BC* bc) {
	for (std::string name : names) {
		auto var = std::make_unique<Variable>();
		var->name = name;
		var->value = bc->builder.CreateAlloca(bc->builder.getInt32Ty(), nullptr, name);

		bc->scopes.back().variables.push_back(std::move(var));
	}

	return true;
}
