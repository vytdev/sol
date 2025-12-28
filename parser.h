/* parse and possibly directly codegen */

#ifndef sol_parser_h
#define sol_parser_h 1

#include "compile.h"


/**
 * parse an identifier reference.
 */
int solP_reference (compiler_t *C);

/**
 * parse an integer.
 */
int solP_int (compiler_t *C);

/**
 * parse primary expressions (i.e., literals, ids, parentheses).
 */
int solP_primary (compiler_t *C);

/**
 * parse binary expression (i.e., add, sub, mul, div).
 */
int solP_binary (compiler_t *C, int min_prec);

/**
 * parse an expression (top-level).
 */
int solP_expr (compiler_t *C);

#endif /* sol_parser_h */
