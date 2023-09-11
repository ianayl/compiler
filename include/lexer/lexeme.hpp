#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>

enum lexeme_t {
	lexeme_null_t = 0,
	lexeme_id_t,
	lexeme_num_lit_t,
	lexeme_plus_t,
	lexeme_dash_t,
	lexeme_asterisk_t,
	lexeme_fslash_t,
	lexeme_caret_t,
	lexeme_lparen_t,
	lexeme_rparen_t,
};

class lexeme {
public:
	lexeme_t type;
	std::string val;
	lexeme(lexeme_t type, std::string val)
	{
		this->type = type;
		this->val = val;
	}
};

#endif
