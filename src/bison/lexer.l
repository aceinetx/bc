%option reentrant bison-bridge
%option noyywrap
%option extra-type="bc::Parser*"

%{
#include "parser.hh"
#include <string>

typedef void* yyscan_t;

#define PARSER (yyget_extra(yyscanner))
%}

%%

[ \t]           ;
[\n]            { PARSER->line_num++; }
"return"        { return RETURN; }
"auto"  	      { return AUTO; }
[0-9]+          { yylval->number = std::stoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* {
	yylval->string = new std::string(yytext);
	return IDENTIFIER;
}
\"([^\\\"\n]|\\.)*\" {
	yylval->string = new std::string(yytext + 1, yytext + yyleng - 1);
  return STRING_LITERAL;
}
"="             { return ASSIGN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"/"             { return DIVIDE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
";"             { return SEMICOLON; }
"&"             { return AMPERSAND; }
","             { return COMMA; }
.               { printf("[bc] unknown token: %s\n", yytext); }

%%
