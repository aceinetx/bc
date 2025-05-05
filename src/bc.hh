#pragma once
#include "result.hh"
#include "yy.hh"
#include <pch.hh>

namespace bc {

typedef struct Variable {
	std::string name;
	llvm::Value* value;
} Variable;

typedef struct Scope {
	std::vector<std::unique_ptr<Variable>> variables;

	Scope();
} Scope;

class BC {
public:
	Parser* parser;
	std::string input;
	std::string compile_error;

	llvm::LLVMContext context;
	llvm::IRBuilder<> builder;
	llvm::Module fmodule;

	Scope global_scope;
	std::vector<Scope> scopes;
	std::stack<llvm::Value*> values;

	BC(std::string moduleName);
	~BC();

	Result<NoSuccess, std::string> parseAndCompile();
	Variable* getVariable(std::string name);

	enum EmitLevel {
		EMIT_OBJ,
		EMIT_IR,
	};

	Result<NoSuccess, std::string> emit(std::string filename, EmitLevel level);
};
} // namespace bc
