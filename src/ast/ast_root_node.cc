#include "ast.hh"

using namespace bc;

void AstRootNode::print(int indent) const {
	printIndent(indent);
	std::cout << "AstRootNode" << std::endl;
	for (const auto& child : children) {
		child->print(indent + 1);
	}
}
