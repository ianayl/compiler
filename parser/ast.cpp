#include "parser/ast.hpp"

#include <iostream>

template<class ast_base, class CRTP>
void* ast_tmpl<ast_base, CRTP>::accept(ast_traversal *t)
{
	return t->visit(static_cast<CRTP*>(this));
}

/*
 * CONCRETE AST NODE DEFINITIONS
 */

ast_epsilon::ast_epsilon() { }
// ast_epsilon::~ast_epsilon() { }

ast_id::ast_id(std::string id) { this->id = id; }

ast_num_lit::ast_num_lit(int val) { this->val = val; }

ast_add::ast_add(ast *lhs, ast *rhs) { this->lhs = lhs; this->rhs = rhs; }

ast_sub::ast_sub(ast *lhs, ast *rhs) { this->lhs = lhs; this->rhs = rhs; }

ast_mul::ast_mul(ast *lhs, ast *rhs) { this->lhs = lhs; this->rhs = rhs; }

ast_div::ast_div(ast *lhs, ast *rhs) { this->lhs = lhs; this->rhs = rhs; }

ast_exp::ast_exp(ast *lhs, ast *rhs) { this->lhs = lhs; this->rhs = rhs; }

ast_neg::ast_neg(ast *operand) { this->operand = operand; }

/*
 * AST_PRINT IMPLEMENTATION
 */

void* ast_print::visit_base_ast(std::string display)
{
	for (int i = 0; i < indent_lvl; i++) std::cout << this->indent_str;
	std::cout << display << std::endl; 
	return 0;
}

void* ast_print::visit_unary_op(std::string display, ast *operand)
{
	for (int i = 0; i < indent_lvl; i++) std::cout << this->indent_str;
	std::cout << display << std::endl; 
	indent_lvl ++;
	operand->accept(this);
	indent_lvl --;
	return 0;
}

void* ast_print::visit_binary_op(std::string display, ast *lhs, ast *rhs)
{
	for (int i = 0; i < indent_lvl; i++) std::cout << this->indent_str;
	std::cout << display << std::endl; 
	indent_lvl ++;
	lhs->accept(this);
	rhs->accept(this);
	indent_lvl --;
	return 0;
}

void* ast_print::visit(ast_epsilon *n) { return visit_base_ast("AST Epsilon"); }

void* ast_print::visit(ast_id *n)
{
	return visit_base_ast("AST Identifier: " + n->id);
}

void* ast_print::visit(ast_num_lit *n)
{
	return visit_base_ast("AST Number Literal: " + std::to_string(n->val));
}

void* ast_print::visit(ast_add *n)
{
	return visit_binary_op("AST Addition (+): ", n->lhs, n->rhs);
}

void* ast_print::visit(ast_sub *n)
{
	return visit_binary_op("AST Subtraction (-): ", n->lhs, n->rhs);
}

void* ast_print::visit(ast_mul *n)
{
	return visit_binary_op("AST Multiplication (*): ", n->lhs, n->rhs);
}

void* ast_print::visit(ast_div *n)
{
	return visit_binary_op("AST Division (/): ", n->lhs, n->rhs);
}

void* ast_print::visit(ast_exp *n)
{
	return visit_binary_op("AST Exponent (^): ", n->lhs, n->rhs);
}

void* ast_print::visit(ast_neg *n)
{
	return visit_unary_op("AST Negation (-): ", n->operand);
}
