/* parse and possibly directly codegen */

#ifndef parser_h
#define parser_h 1

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
void parser_init(parser_t *parser, char *src, int len);

/**
 * free up the parser.
 */
void parser_free(parser_t *parser);

/**
 * parse an identifier reference.
 */
int parse_reference(parser_t *parser);

/**
 * parse an integer.
 */
int parse_int(parser_t *parser);

/**
 * parse primary expressions (i.e., literals, ids, parentheses).
 */
int parse_primary(parser_t *parser);

/**
 * parse binary expression (i.e., add, sub, mul, div).
 */
int parse_binary(parser_t *parser, int min_prec);

/**
 * parse an expression (top-level).
 */
int parse_expr(parser_t *parser);

#endif /* parser_h */
