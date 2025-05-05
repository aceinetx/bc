#include "bc.hh"
#include <parser.hh>

typedef void* yyscan_t;
void yylex_init(yyscan_t*);
void yylex_destroy(yyscan_t);
void yyset_extra(bc::Parser* user_defined, void* yyscanner);

using namespace bc;
using namespace llvm;

Scope::Scope() {
	variables.clear();
}

BC::BC(std::string moduleName) : builder(context), fmodule(moduleName, context) {
	InitializeNativeTarget();
	InitializeAllTargetInfos();
	InitializeAllTargets();
	InitializeAllTargetMCs();
	InitializeAllAsmParsers();
	InitializeAllAsmPrinters();

	parser = new Parser();
}

BC::~BC() {
	if (parser)
		delete parser;
}

Result<NoSuccess, std::string> BC::parseAndCompile() {
	yyscan_t scanner;
	YY_BUFFER_STATE buffer;

	yylex_init(&scanner);
	yyset_extra(this->parser, scanner);

	buffer = yy_scan_string(input.c_str(), scanner);
	yyparse(scanner, this->parser);

	if (!parser->root) {
		return Result<NoSuccess, std::string>::error(fmt::format("root == nullptr (parse error: {})", parser->error));
	}

	yy_delete_buffer(buffer, scanner);
	yylex_destroy(scanner);

	if (!parser->root->compile(this)) {
		return Result<NoSuccess, std::string>::error(fmt::format("compile error: {}", compile_error));
	}

	delete parser->root;

	verifyModule(fmodule);

	return Result<NoSuccess, std::string>::success({});
}

Variable* BC::getVariable(std::string name) {
	for (Scope& scope : scopes) {
		for (std::unique_ptr<Variable>& var : scope.variables) {
			if (var->name != name)
				continue;

			return var.get();
		}
	}
	return nullptr;
}

Result<NoSuccess, std::string> BC::emit(std::string filename, EmitLevel level) {
#define ERROR(...) return Result<NoSuccess, std::string>::error(fmt::format(__VA_ARGS__))
#define SUCCESS(...) return Result<NoSuccess, std::string>::success({})

	switch (level) {
	case EmitLevel::EMIT_OBJ: {
		std::string targetTriple = sys::getDefaultTargetTriple();

		std::string error;
		auto target = TargetRegistry::lookupTarget(targetTriple, error);
		if (!target) {
			ERROR("target lookup error: {}", error);
		}

		auto cpu = "generic";
		auto features = "";

		TargetOptions opt;
		auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, Reloc::PIC_);

		fmodule.setDataLayout(targetMachine->createDataLayout());
		fmodule.setTargetTriple(targetTriple);

		std::error_code EC;
		raw_fd_ostream dest(filename, EC, sys::fs::OF_None);
		if (EC) {
			ERROR("could not open output file: {}", EC.message());
		}

		legacy::PassManager pass;
		auto fileType = CodeGenFileType::ObjectFile;
		if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
			ERROR("TargetMachine can't emit a file of this type");
		}

		pass.run(fmodule);
		dest.flush();

		SUCCESS();
	} break;
	case EmitLevel::EMIT_IR: {
		std::error_code EC;
		raw_fd_ostream dest(filename, EC, sys::fs::OF_None);
		if (EC) {
			ERROR("could not open output file: {}", EC.message());
		}

		fmodule.print(dest, nullptr);
		SUCCESS();
	} break;
	}

	ERROR("unreachable");
#undef ERROR
#undef SUCCESS
}
