%define api.pure full
%lex-param { yyscan_t scanner }
%parse-param { void *scanner }
%parse-param { bc::Parser *ctx }

%{
#include <pch.hh>
#include <src/ast/ast.hh>
#include <parser.hh>

typedef void* yyscan_t;
int yylex(YYSTYPE *yylval_param, yyscan_t scanner);
void yyerror(yyscan_t scanner,bc::Parser *ctx, const char *msg);

#define PARSER (yyget_extra(scanner))
%}

%code requires {
#include <src/ast/ast.hh>

namespace bc {
	struct Parser {
		unsigned long line_num = 1;
		bc::AstRootNode* root = nullptr;
		std::string error = "";
	};
}

bc::Parser* yyget_extra(void*);

}

%union {
	int number;
	std::string *string;
	bc::AstNode* node;
	bc::AstNode* stmt;

	std::vector<bc::AstNode*>* top_stmt_list;

	std::vector<bc::AstNode*>* stmt_list;
	std::vector<bc::AstNode*>* func_list;
	
	std::vector<std::string>* identifier_list;
}

%token <number> NUMBER
%token <string> IDENTIFIER
%token <string> STRING_LITERAL
%token RETURN AUTO
%token ASSIGN PLUS MINUS MULTIPLY DIVIDE
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON AMPERSAND COMMA

%type <node> program function_definition
%type <node> expression term factor
%type <node> statement declaration assignment return_statement
%type <stmt_list> statement_list
%type <node> topstatement
%type <top_stmt_list> topstatements
%type <identifier_list> identifier_list


%left PLUS MINUS
%left MULTIPLY DIVIDE

%%

program:
	topstatements {
		PARSER->root = new bc::AstRootNode();
		for (const auto& func : *$1) {
			PARSER->root->children.push_back(func);
		}
		delete $1;
	}
	;

function_definition:
	IDENTIFIER LPAREN RPAREN LBRACE statement_list RBRACE {
		auto* func = new bc::AstFuncDef();
		func->name = *$1;
		for (const auto& stmt : *$5) {
			func->children.push_back(stmt);
		}
		delete $5;
		delete $1;
		$$ = func;
	}
	;

topstatements:
	topstatement {
		$$ = new std::vector<bc::AstNode*>();
		$$->push_back($1);
	} | topstatements topstatement {
		$1->push_back($2);
		$$ = $1;
	}
	;

topstatement:
	function_definition
	;

statement_list:
	statement {
		$$ = new std::vector<bc::AstNode*>();
		$$->push_back($1);
	}
	| statement_list statement {
		$1->push_back($2);
		$$ = $1;
	}
	| {
		$$ = new std::vector<bc::AstNode*>();
	}
	;

statement:
	declaration
	| assignment
	| return_statement
	;

declaration:
	AUTO identifier_list SEMICOLON {
		auto* decl = new bc::AstVarDeclare();
		decl->names = *$2;
		delete $2;
		$$ = decl;
	}
	;

identifier_list:
	IDENTIFIER {
		$$ = new std::vector<std::string>();
		$$->push_back(*$1);
		delete $1;
	} | identifier_list COMMA IDENTIFIER {
    $1->push_back(*$3);
    $$ = $1;
		delete $3;
	}

assignment:
	IDENTIFIER ASSIGN expression SEMICOLON {
		auto* assign = new bc::AstVarAssign();
		assign->name = *$1;
		assign->expr = $3;
		delete $1;
		$$ = assign;
	}
	;

return_statement:
	RETURN expression SEMICOLON {
		auto* ret = new bc::AstReturn();
		ret->expr = $2;
		$$ = ret;
	}
	| RETURN SEMICOLON {
		auto* ret = new bc::AstReturn();
		$$ = ret;
	}
	;

expression:
	term
	| expression PLUS term {
		auto* op = new bc::AstBinaryOp();
		op->left = $1;
		op->right = $3;
		op->op = "add";
		$$ = op;
	}
	| expression MINUS term {
		auto* op = new bc::AstBinaryOp();
		op->left = $1;
		op->right = $3;
		op->op = "sub";
		$$ = op;
	}
	;

term:
	factor
	| term MULTIPLY factor {
		auto* op = new bc::AstBinaryOp();
		op->left = $1;
		op->right = $3;
		op->op = "mul";
		$$ = op;
	}
	| term DIVIDE factor {
		auto* op = new bc::AstBinaryOp();
		op->left = $1;
		op->right = $3;
		op->op = "div";
		$$ = op;
	}
	;

factor:
	NUMBER {
		$$ = new bc::AstNumber($1);
	}
	| IDENTIFIER {
		auto* var = new bc::AstVarRef();
		var->name = *$1;
		delete $1;
		$$ = var;
	}
	| LPAREN expression RPAREN {
		$$ = $2;
	}
	;

%%

void yyerror([[maybe_unused]] yyscan_t scanner,bc::Parser *ctx, const char *s) {
	ctx->error = fmt::format("error at line {}: {}", ctx->line_num, s);
}
