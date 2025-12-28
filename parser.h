/* parse and possibly directly codegen */

#ifndef solcP_h
#define solcP_h 1

#include "solc.h"


/**
 * parse an identifier reference.
 */
int solcP_reference (solc *C);

/**
 * parse an integer.
 */
int solcP_int (solc *C);

/**
 * parse primary expressions (i.e., literals, ids, parentheses).
 */
int solcP_primary (solc *C);

/**
 * parse postfix unary ops.
 */
int solcP_postfix (solc *C);

/**
 * parse prefix unary ops (i.e., negation, plus op).
 */
int solcP_prefix (solc *C);

/**
 * parse binary expression (i.e., add, sub, mul, div).
 */
int solcP_infix (solc *C, int min_prec);

/**
 * parse an expression (top-level).
 */
int solcP_expr (solc *C);

#endif /* solcP_h */
