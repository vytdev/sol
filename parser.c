#include "parser.h"
#include "lexer.h"
#include "util.h"


uint64_t parse_int(parser_t *parser, lexer_t *lex) {
  token_t tok = lexer_consume(lex);
  if (tok.type != T_INTEGER) {
    msgtok(&tok, lex, "error: Expected an integer\n");
    parser->err = 1;
    return 0;
  }

  // simply parse a base 10 uint. we don't care about the sign here
  uint64_t val = 0;
  for (int i = 0; i < tok.len; i++) {
    char digit = tok.start[i] - '0';
    // efficiently prevent overflow
    if (val > UINT64_MAX / 10 || (val == UINT64_MAX / 10
        && digit > UINT64_MAX % 10)) {
      msgtok(&tok, lex, "error: Integer too big to fit into 64-bits\n");
      parser->err = 1;
      return 0;
    }
    val = val * 10 + digit;
  }

  return val;
}
