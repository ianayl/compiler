#include "lexer/lexeme.hpp"
#include "parser/ast.hpp"
#include "parser/recdec_parser.hpp"
#include <vector>

int main()
{
	// ast *n2 = new ast_add(new ast_num_lit(1), new ast_id("johnathan"));
	// ast *n3 = new ast_mul(new ast_num_lit(2), new ast_add(new ast_num_lit(3), new ast_num_lit(4)));
	// ast *n1 = new ast_add(n2, n3);

	
	std::vector<lexeme> poop;
	poop.push_back(lexeme(lexeme_id_t, "john"));
	poop.push_back(lexeme(lexeme_asterisk_t, "*"));
	// poop.push_back(lexeme(lexeme_lparen_t, "("));
	poop.push_back(lexeme(lexeme_dash_t, "-"));
	poop.push_back(lexeme(lexeme_dash_t, "-"));
	poop.push_back(lexeme(lexeme_num_lit_t, "1"));
	poop.push_back(lexeme(lexeme_dash_t, "-"));
	poop.push_back(lexeme(lexeme_num_lit_t, "3"));
	// poop.push_back(lexeme(lexeme_rparen_t, ")"));
	poop.push_back(lexeme(lexeme_caret_t, "^"));
	poop.push_back(lexeme(lexeme_num_lit_t, "5"));
	poop.push_back(lexeme(lexeme_fslash_t, "/"));
	poop.push_back(lexeme(lexeme_num_lit_t, "4"));
	// poop.push_back(lexeme(lexeme_num_lit_t, "4"));
	// poop.push_back(lexeme(lexeme_caret_t, "^"));
	// poop.push_back(lexeme(lexeme_num_lit_t, "5"));
	parser *p = new recdec_parser(&poop);
	ast* res = p->parse();

	ast_print cum;
	cum.traverse(res);
	delete res;
}
