#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <iostream>

/**
 * enum type to label each AST node with it's designated type
 */
enum ast_t
{
	ast_null_t = 0,
	ast_epsilon_t,
	ast_id_t,
	ast_num_lit_t,
	ast_add_t,
	ast_sub_t,
	ast_mul_t,
	ast_div_t,
	ast_exp_t,
	ast_neg_t
};

/**
 * Base ABSTRACT class for a simple AST node
 */
class ast {
public:
	ast_t type;
	virtual void* accept(class ast_traversal *t) = 0;
	virtual ~ast() {} // TODO DELETE THE ENTIRE TREE!

	// TODO this is inefficient memory usage. you could probably do some bitwise
	// math to make this value smaller. Then, use static methods to figure out
	// if something is what.
	virtual bool is_binary_op() { return false; }
	virtual bool is_unary_op() { return false; }
};

/**
 * Base ABSTRACT class for a AST node denoting an unary operation 
 * (left/right agnostic)
 */
class ast_unary_op : public ast {
public:
	ast *operand; /* Right hand side */
	virtual bool is_unary_op() { return true; }
};

/**
 * Base ABSTRACT class for a AST node denoting a binary operation
 */
class ast_binary_op : public ast {
public:
	ast *lhs; /* Left hand side */
	ast *rhs; /* Right hand side */
	bool is_binary_op() { return true; }
};

/**
 * Template class used to declare an AST node while implementing the visitor
 * pattern
 */
template<class ast_base, class CRTP>
class ast_tmpl : public ast_base {
public:
	/**
	 * Traverse the current AST node using the given AST traversal
	 * @param t Pointer to an AST traversal used to traverse this AST node
	 */
	void* accept(ast_traversal *t);
};

/*
 * CONCRETE AST NODE DEFINITIONS START HERE
 */

/**
 * AST node representing epsilon -- used for parsing, SHOULD NOT ACTUALLY END UP
 * IN FINISHED CODE
 */
class ast_epsilon : public ast_tmpl<ast, ast_epsilon> {
public:
	ast_t type = ast_epsilon_t;
	ast_epsilon();
};

/** AST node representing an identifier */
class ast_id : public ast_tmpl<ast, ast_id> {
public:
	ast_t type = ast_id_t;
	std::string id;
	ast_id(std::string id);
};

/** AST node representing a numeric literal */
class ast_num_lit : public ast_tmpl<ast, ast_num_lit> {
public:
	ast_t type = ast_num_lit_t;
	int val;
	ast_num_lit(int val);
};

/** AST node representing the addition operation */
class ast_add : public ast_tmpl<ast_binary_op, ast_add> {
public:
	ast_t type = ast_add_t;
	ast_add(ast *lhs, ast *rhs);
};

/** AST node representing the subtraction operation */
class ast_sub : public ast_tmpl<ast_binary_op, ast_sub> {
public:
	ast_t type = ast_sub_t;
	ast_sub(ast *lhs, ast *rhs);
};

/** AST node representing the multiplication operation */
class ast_mul : public ast_tmpl<ast_binary_op, ast_mul> {
public:
	ast_t type = ast_mul_t;
	ast_mul(ast *lhs, ast *rhs);
};

/** AST node representing the multiplication operation */
class ast_div : public ast_tmpl<ast_binary_op, ast_div> {
public:
	ast_t type = ast_div_t;
	ast_div(ast *lhs, ast *rhs);
};

/** AST node representing the multiplication operation */
class ast_exp : public ast_tmpl<ast_binary_op, ast_exp> {
public:
	ast_t type = ast_exp_t;
	ast_exp(ast *lhs, ast *rhs);
};

class ast_neg : public ast_tmpl<ast_unary_op, ast_neg> {
public:
	ast_t type = ast_neg_t;
	ast_neg(ast *operand);
};

/*
 * AST TRAVERSAL DEFINITIONS
 */

/**
 * NOTE: use ast_traversal_tmpl when declaring AST traversals - Return types are
 * enforced independently in the traverse() function and each individual 
 * implementation of the visit function.
 *
 * Base class for a visitor pattern representing one tree-walk traversal of
 * the AST -- useful for e.g. AST optimization, type checking, or straight tree-
 * walk interpretation.
 */
class ast_traversal {
public:
	virtual void* visit(ast_epsilon *n) = 0;
	virtual void* visit(ast_id      *n) = 0;
	virtual void* visit(ast_num_lit *n) = 0;
	virtual void* visit(ast_add     *n) = 0;
	virtual void* visit(ast_sub     *n) = 0;
	virtual void* visit(ast_mul     *n) = 0;
	virtual void* visit(ast_div     *n) = 0;
	virtual void* visit(ast_exp     *n) = 0;
	virtual void* visit(ast_neg     *n) = 0;
};
/*
 * Note on the visitor functions:
 * - accept() is a CRTP in order to call the function as a specific ast type
 * - visit() is the visitor function: The return type is void* to allow multiple
 *   return types that is ultimately enforced INTERNALLY in each of the visit
 *   functions and the ast_traversal_tmpl class.
 * - traverse() is the ACTUAL function used to traverse an AST. As with above,
 *   traverse in ast_traversal_tmpl will actually enforce the return result.
 */

/**
 * Template class for declaring an ast traversal: Used to enforce return types
 */
template<typename result_t>
class ast_traversal_tmpl : public ast_traversal {
public:
	/**
	 * Traverse the AST using the current AST traversal.
	 * @param n Pointer to the AST node to traverse.
	 */
	result_t traverse(ast *n) {
		return static_cast<result_t>(n->accept(this)); /* Simple cast */
	}
};

/*
 * CONCRETE AST TRAVERSALS BEGIN HERE
 */

/**
 * AST Traversal for printing an AST to stdout
 */
class ast_print : public ast_traversal_tmpl<void> {
private:
	int indent_lvl = 0;
	std::string indent_str = "    ";

	void* visit_base_ast(std::string display);
	void* visit_binary_op(std::string display, ast *lhs, ast *rhs);
	void* visit_unary_op(std::string display, ast *operand);
public:
	virtual void* visit(ast_epsilon *n);
	virtual void* visit(ast_id      *n);
	virtual void* visit(ast_num_lit *n);
	virtual void* visit(ast_add     *n);
	virtual void* visit(ast_sub     *n);
	virtual void* visit(ast_mul     *n);
	virtual void* visit(ast_div     *n);
	virtual void* visit(ast_exp     *n);
	virtual void* visit(ast_neg     *n);
};


#endif
