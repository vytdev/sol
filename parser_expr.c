#include "parser.h"
#include "util.h"
#include <stdint.h>


int parse_reference(parser_t *parser)
{
  token_t tok = lexer_consume(&parser->lexer);
  if (tok.type != T_IDENTIFIER) {
    msgtok(&tok, &parser->lexer, "syntax error: Expected identifier\n");
    return PFAIL;
  }
  // TODO: codegen
  return PSUCC;
}


int parse_int(parser_t *parser)
{
  token_t tok = lexer_consume(&parser->lexer);
  if (tok.type != T_INTEGER) {
    msgtok(&tok, &parser->lexer, "syntax error: Expected an integer\n");
    return PFAIL;
  }

  // simply parse a base 10 uint. we don't care about the sign here
  uint64_t val = 0;
  for (int i = 0; i < tok.len; i++) {
    char digit = tok.start[i] - '0';
    // efficiently prevent overflow
    if (val > UINT64_MAX / 10 || (val == UINT64_MAX / 10
        && digit > UINT64_MAX % 10)) {
      msgtok(&tok, &parser->lexer,
             "syntax error: Integer too big to fit into 64-bits\n");
      return PFAIL;
    }
    val = val * 10 + digit;
  }

  // TODO: codegen
  return PSUCC;
}


int parse_primary(parser_t *parser)
{
  token_t peek = lexer_peek(&parser->lexer);
  switch (peek.type) {
    case T_IDENTIFIER:
      return parse_reference(parser);
    case T_INTEGER:
      return parse_int(parser);
    case T_LPAREN: {
      lexer_consume(&parser->lexer);
      if (parse_expr(parser) != PSUCC)
        return PFAIL;
      peek = lexer_consume(&parser->lexer);
      if (peek.type != T_RPAREN) {
        msgtok(&peek, &parser->lexer,
               "syntax error: Expected ')' to close '('\n");
        return PFAIL;
      }
      return PSUCC;
    }
    default:
      msgtok(&peek, &parser->lexer, "syntax error: Expected expression\n");
      return PFAIL;
  }
}


#define BIN_UNK   (0)
#define BIN_ADD   (1)
#define BIN_SUB   (2)
#define BIN_MUL   (3)
#define BIN_DIV   (4)

/*
 * get binop (BIN_*) based on token type (T_*)
 */
static int tok2binop(int ttype)
{
  switch (ttype) {
    case T_PLUS:      return BIN_ADD;
    case T_DASH:      return BIN_SUB;
    case T_STAR:      return BIN_MUL;
    case T_SLASH:     return BIN_DIV;
    default:          return BIN_UNK;
  }
}

#define LASSOC    (1)
#define RASSOC    (0)
struct binopinfo {
  int prec;
  int assoc;
};

/* infix op infos for bottom-up parsing */
static const struct binopinfo binops[] = {
    [BIN_UNK]   = { -1, -1 },
    [BIN_ADD]   = { 1, LASSOC },
    [BIN_SUB]   = { 1, LASSOC },
    [BIN_MUL]   = { 2, LASSOC },
    [BIN_DIV]   = { 2, LASSOC },
};


int parse_binary(parser_t *parser, int min_prec)
{
  // parse initial lhs
  if (parse_primary(parser) != PSUCC)
    return PFAIL;

  for (;;) {
    // check if next token is a binary op.
    token_t peek = lexer_peek(&parser->lexer);
    int op = tok2binop(peek.type);
    if (op == BIN_UNK)
      break;

    // get binopinfo. check if prec is too low than min.
    const struct binopinfo *info = &binops[op];
    if (info->prec < min_prec)
      break;
    lexer_consume(&parser->lexer);   // consume op.

    // parse rhs
    if (parse_binary(parser, info->prec + info->assoc) != PSUCC)
      return PFAIL;

    // TODO: codegen (op instr)
  }

  return PSUCC;
}


int parse_expr(parser_t *parser)
{
  return parse_binary(parser, 0);
}
