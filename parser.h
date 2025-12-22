/* parse and possibly directly codegen */

#ifndef PARSER_H_
#define PARSER_H_ 1

#include "lexer.h"
#include <stdint.h>

typedef struct parser parser_t;
struct parser {
  int err;  /* should continue? */
};

#define PARSER_INIT ((parser_t){0})

/**
 * parse an integer.
 */
uint64_t parse_int(parser_t *parser, lexer_t *lex);

#endif /* PARSER_H_ */
