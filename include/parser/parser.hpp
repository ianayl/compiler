#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "lexer/lexeme.hpp"
#include "parser/ast.hpp"

class parser {
protected:
	std::vector<lexeme> *input;
	ast *epsilon;

public:
	parser(std::vector<lexeme> *input)
	{
		this->input = input;
		epsilon = new ast_epsilon();
	}

	~parser() { } //delete (ast_epsilon*) epsilon; }

	virtual ast* parse() = 0;
};

#endif
