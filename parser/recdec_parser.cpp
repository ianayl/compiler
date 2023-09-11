#include "parser/recdec_parser.hpp"
#include "lexer/lexeme.hpp"
#include "parser/ast.hpp"

lexeme recdec_parser::peek(int lookahead)
{
	if (input->size() <= lookahead)
		return lexeme(lexeme_null_t, "");
	return (*input)[lookahead];
}

lexeme recdec_parser::consume(int lookahead)
{
	if (input->size() <= lookahead)
		return lexeme(lexeme_null_t, "");
	// lexeme tmp = lexeme((*input)[lookahead].type, (*input)[lookahead].val) ;
	lexeme tmp = (*input)[lookahead];
	input->erase(input->begin(), input->begin() + lookahead + 1);
	return tmp;
}

ast_binary_op* recdec_parser::get_binop_successor(ast *n)
{
	/* Descend down the tree to find successor: */
	while (n->is_binary_op() && ((ast_binary_op*) n)->lhs != nullptr)
		n = ((ast_binary_op*) n)->lhs;

	// || (n->is_binary_op() && ((ast_binary_op*) n)->lhs != nullptr)
	if (!(n->is_binary_op()) ) return nullptr; // TODO FAIL HERE
	else return (ast_binary_op*) n;
}

ast* recdec_parser::parse()
{
	// TODO FINISH THIS FUNCTION DINGUS
	for (int i = 0; i < input->size(); i++) {
		std::cout << (*input)[i].val << " " ;
	}
	std::cout << std::endl;
	return parse_expr();
}

ast* recdec_parser::parse_expr()
{
	/* Expr ::= Add */
	ast *tmp = parse_add();
	if (tmp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	return tmp;
}

ast* recdec_parser::parse_add()
{
	/* Add  ::= Mul Addp */
	ast *mul = parse_mul();
	ast_binary_op *addp = (ast_binary_op*) parse_addp();
	if (mul == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	if (addp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	/* Checking that Addp isn't epsilon */
	else if (addp == epsilon) return mul;
	else {
		ast_binary_op *tmp = get_binop_successor(addp);
		if (tmp == nullptr) return nullptr; // TODO FAIL HERE
		tmp->lhs = mul;
		return addp;
	}
}

ast* recdec_parser::parse_addp()
{
	ast_binary_op *res = nullptr;
	if (peek().type == lexeme_plus_t) res = new ast_add(nullptr, nullptr);
	else if (peek().type == lexeme_dash_t) res = new ast_sub(nullptr, nullptr);
	else return epsilon;
	consume();

	ast* mul = parse_mul();
	ast_binary_op *addp = (ast_binary_op*) parse_addp();
	if (mul == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	if (addp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	/* Checking that Addp isn't epsilon */
	else if (addp == epsilon) {
		res->rhs = mul;
		return res;
	} else {
		ast_binary_op *tmp = get_binop_successor(addp);
		if (tmp == nullptr) return nullptr; // TODO FAIL HERE
		tmp->lhs = res;
		res->rhs = mul;
		return addp;
	}
}

ast* recdec_parser::parse_mul()
{
	/* Mul  ::= Exp Mulp */
	ast *exp = parse_exp();
	ast_binary_op *mulp = (ast_binary_op*) parse_mulp();
	if (exp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	if (mulp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	/* Checking that Addp isn't epsilon */
	else if (mulp == epsilon) return exp;
	else {
		ast_binary_op *tmp = get_binop_successor(mulp);
		if (tmp == nullptr) return nullptr; // TODO FAIL HERE
		tmp->lhs = exp;
		return mulp;
	}
}

ast* recdec_parser::parse_mulp()
{
	ast_binary_op *res = nullptr;
	if (peek().type == lexeme_asterisk_t) res = new ast_mul(nullptr, nullptr);
	else if (peek().type == lexeme_fslash_t) res = new ast_div(nullptr, nullptr);
	else return epsilon;
	consume();

	ast* exp = parse_exp();
	ast_binary_op *mulp = (ast_binary_op*) parse_mulp();
	if (exp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	if (mulp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	/* Checking that Mulp isn't epsilon */
	else if (mulp == epsilon) {
		res->rhs = exp;
		return res;
	} else {
		ast_binary_op *tmp = get_binop_successor(mulp);
		if (tmp == nullptr) return nullptr; // TODO FAIL HERE
		tmp->lhs = res;
		res->rhs = exp;
		return mulp;
	}
}

ast* recdec_parser::parse_exp()
{
	/* Mul  ::= Exp Mulp */
	ast *term = parse_term();
	ast_binary_op *expp = (ast_binary_op*) parse_expp();
	if (term == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	if (expp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	/* Checking that Expp isn't epsilon */
	else if (expp == epsilon) return term;
	else {
		ast_binary_op *tmp = get_binop_successor(expp);
		if (tmp == nullptr) return nullptr; // TODO FAIL HERE
		tmp->lhs = term;
		return expp;
	}
}

ast* recdec_parser::parse_expp()
{
	ast_binary_op *res = nullptr;
	if (peek().type == lexeme_caret_t) res = new ast_exp(nullptr, nullptr);
	else return epsilon;
	consume();

	ast* term = parse_term();
	ast_binary_op *expp = (ast_binary_op*) parse_expp();
	if (term == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	if (expp == nullptr) return nullptr; // TODO FAILURE FUNCTION HERE
	/* Checking that Mulp isn't epsilon */
	else if (expp == epsilon) {
		res->rhs = term;
	} else {
		ast_binary_op *tmp = get_binop_successor(expp);
		if (tmp == nullptr) return nullptr; // TODO FAIL HERE
		tmp->lhs = term;
		res->rhs = expp;
	}
	return res;
}

ast* recdec_parser::parse_term()
{
	/* Term ::= ( Expr ) */
	if (peek().type == lexeme_lparen_t) {
		consume();
		ast *expr = parse_expr();
		if (expr == nullptr) return nullptr; // TODO FAILURE HERE
		else if (peek().type == lexeme_rparen_t) {
			consume();
			return expr;
		} else return nullptr; // TODO failure here
	/* Term ::= '-' Term */
	} else if (peek().type == lexeme_dash_t) {
		consume();
		ast *term = parse_term();
		if (term == nullptr) return nullptr; // TODO FAILURE HERE
		else return new ast_neg(term);
	/* Term ::= <id> */
	} else if (peek().type == lexeme_id_t) {
		std::string id = consume().val;
		return new ast_id(id);
	/* Term ::= <num_lit> */
	} else if (peek().type == lexeme_num_lit_t) {
		int val = stoi(consume().val);
		return new ast_num_lit(val);
	} else return nullptr; // TODO failure here
}
