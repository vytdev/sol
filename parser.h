/* parse and possibly directly codegen */

#ifndef sol_parser_h
#define sol_parser_h 1

#include "compile.h"


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
 * parse binary expression (i.e., add, sub, mul, div).
 */
int solcP_binary (solc *C, int min_prec);

/**
 * parse an expression (top-level).
 */
int solcP_expr (solc *C);

#endif /* sol_parser_h */
