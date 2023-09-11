#ifndef RECDEC_PARSER_HPP
#define RECDEC_PARSER_HPP

#include "parser/parser.hpp"

class recdec_parser : public parser {
private:
	lexeme peek(int lookahead = 0);
	lexeme consume(int lookahead = 0);
	ast_binary_op* get_binop_successor(ast *n);

	ast* parse_expr();
	ast* parse_add();
	ast* parse_addp();
	ast* parse_mul();
	ast* parse_mulp();
	ast* parse_exp();
	ast* parse_expp();
	ast* parse_term();
public:
	recdec_parser(std::vector<lexeme> *input) : parser(input) {  }
	ast* parse();
};

#endif
