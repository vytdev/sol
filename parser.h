/* parse and possibly directly codegen */

#ifndef sol_parser_h
#define sol_parser_h 1

#include "lexer.h"


struct parse_err {
  char *msg;
  token_t token;
};

/* how much parse error before stopping the parsing. */
#define MAXPARSEERR 20

typedef struct parser parser_t;
struct parser {
  lexer_t lex;
  struct parse_err err[MAXPARSEERR];
};

#define PSUCC  (0)
#define PFAIL  (1)

/**
 * initialize a parser context.
 */
void solP_init (parser_t *P, char *src, int len);

/**
 * free up the parser.
 */
void solP_free (parser_t *P);

/**
 * add a parse error msg.
 */
int solP_err (parser_t *P, int cnt, token_t *tok, char *msg);

/**
 * parse an identifier reference.
 */
int solP_reference (parser_t *P);

/**
 * parse an integer.
 */
int solP_int (parser_t *P);

/**
 * parse primary expressions (i.e., literals, ids, parentheses).
 */
int solP_primary (parser_t *P);

/**
 * parse binary expression (i.e., add, sub, mul, div).
 */
int solP_binary (parser_t *P, int min_prec);

/**
 * parse an expression (top-level).
 */
int solP_expr (parser_t *P);

#endif /* sol_parser_h */
