#include "ast.hh"
#include <pch.hh>

using namespace bc;

AstNode::~AstNode() {
	for (AstNode* node : children) {
		delete node;
	}
}

void AstNode::printIndent(int indent) const {
	for (int i = 0; i < indent; ++i) {
		std::cout << "  ";
	}
}

bool AstNode::compile(BC* bc) {
	for (AstNode* node : children) {
		if (!node->compile(bc))
			return false;
	}
	return true;
}
