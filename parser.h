/* parse and possibly directly codegen */

#ifndef sol_parser_h
#define sol_parser_h 1

#include "lexer.h"


typedef struct parser parser_t;
struct parser {
  lexer_t lexer;
};

#define PSUCC  (0)
#define PFAIL  (1)

/**
 * initialize a parser context.
 */
void solP_init (parser_t *parser, char *src, int len);

/**
 * free up the parser.
 */
void solP_free (parser_t *parser);

/**
 * parse an identifier reference.
 */
int solP_reference (parser_t *parser);

/**
 * parse an integer.
 */
int solP_int (parser_t *parser);

/**
 * parse primary expressions (i.e., literals, ids, parentheses).
 */
int solP_primary (parser_t *parser);

/**
 * parse binary expression (i.e., add, sub, mul, div).
 */
int solP_binary (parser_t *parser, int min_prec);

/**
 * parse an expression (top-level).
 */
int solP_expr (parser_t *parser);

#endif /* sol_parser_h */
