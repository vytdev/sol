/* parse and possibly directly codegen */

#ifndef sol_parser_h
#define sol_parser_h 1

#include "ast.h"
#include "arena.h"
#include "lexer.h"
#include <stdlib.h> /* for NULL. */
#include <stdint.h>


typedef struct parser parser_t;
struct parser {
  arena_t *arena;
  lexer_t lexer;
  int err_cnt;
};

/* internal: for parser_* funcs. */
#define newnode(t) ((t *)arena_new(parser->arena, sizeof(t)))

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
ast_expr *parse_reference(parser_t *parser);

/**
 * parse an integer.
 */
ast_expr *parse_int(parser_t *parser);

/**
 * parse primary expressions (i.e., literals, ids, parentheses).
 */
ast_expr *parse_primary(parser_t *parser);

/**
 * parse binary expression (i.e., add, sub, mul, div).
 */
ast_expr *parse_binary(parser_t *parser, int min_prec);

/**
 * parse an expression (top-level).
 */
ast_expr *parse_expr(parser_t *parser);

#endif /* sol_parser_h */
