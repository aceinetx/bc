#include "../bc.hh"
#include "ast.hh"

using namespace bc;
using namespace llvm;

void AstBinaryOp::print(int indent) const {
	printIndent(indent);
	std::cout << "AstBinaryOp" << std::endl;
	printIndent(indent + 1);
	std::cout << "op: " << op << std::endl;
	printIndent(indent + 1);
	std::cout << "left:" << std::endl;
	left->print(indent + 2);
	printIndent(indent + 1);
	std::cout << "right:" << std::endl;
	right->print(indent + 2);
}

AstBinaryOp::~AstBinaryOp() {
	delete left;
	delete right;
}

bool AstBinaryOp::compile(BC* bc) {
	if (!left->compile(bc))
		return false;
	if (!right->compile(bc))
		return false;

	Value* RHS = bc->values.top();
	bc->values.pop();
	Value* LHS = bc->values.top();
	bc->values.pop();

	Value* result = nullptr;

	if (op == "add") {
		result = bc->builder.CreateAdd(LHS, RHS);
	} else if (op == "sub") {
		result = bc->builder.CreateSub(LHS, RHS);
	} else if (op == "mul") {
		result = bc->builder.CreateMul(LHS, RHS);
	} else if (op == "div") {
		bc->compile_error = "div is a TODO";
		return false;
	}

	bc->values.push(result);

	return true;
}
