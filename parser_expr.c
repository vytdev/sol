#include "parser.h"
#include <stdint.h>


int solP_reference (parser_t *P)
{
  token_t tok = solL_consume(&P->lex);
  if (tok.type != T_IDENTIFIER)
    return solP_err(P, 0, &tok, "syntax error: Expected identifier\n");
  // TODO: codegen
  return PSUCC;
}


int solP_int (parser_t *P)
{
  token_t tok = solL_consume(&P->lex);
  if (tok.type != T_INTEGER)
    return solP_err(P, 0, &tok, "syntax error: Expected an integer\n");

  // simply parse a base 10 uint. we don't care about the sign here
  uint64_t val = 0;
  for (int i = 0; i < tok.len; i++) {
    char digit = tok.start[i] - '0';
    // efficiently prevent overflow
    if (val > UINT64_MAX / 10 || (val == UINT64_MAX / 10
        && digit > UINT64_MAX % 10))
      return solP_err(P, 0, &tok,
             "syntax error: Integer too big to fit into 64-bits\n");
    val = val * 10 + digit;
  }

  // TODO: codegen
  return PSUCC;
}


int solP_primary (parser_t *P)
{
  token_t peek = solL_peek(&P->lex);
  switch (peek.type) {
    case T_IDENTIFIER:
      return solP_reference(P);
    case T_INTEGER:
      return solP_int(P);
    case T_LPAREN: {
      solL_consume(&P->lex);
      if (solP_expr(P) != PSUCC)
        return PFAIL;
      peek = solL_consume(&P->lex);
      if (peek.type != T_RPAREN)
        return solP_err(P, 0, &peek,
                        "syntax error: Expected ')' to close '('\n");
      return PSUCC;
    }
    default:
      return solP_err(P, 0, &peek, "syntax error: Expected expression\n");
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
static int tok2binop (int ttype)
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


int solP_binary (parser_t *P, int min_prec)
{
  // parse initial lhs
  if (solP_primary(P) != PSUCC)
    return PFAIL;

  for (;;) {
    // check if next token is a binary op.
    token_t peek = solL_peek(&P->lex);
    int op = tok2binop(peek.type);
    if (op == BIN_UNK)
      break;

    // get binopinfo. check if prec is too low than min.
    const struct binopinfo *info = &binops[op];
    if (info->prec < min_prec)
      break;
    solL_consume(&P->lex);   // consume op.

    // parse rhs
    if (solP_binary(P, info->prec + info->assoc) != PSUCC)
      return PFAIL;

    // TODO: codegen (op instr)
  }

  return PSUCC;
}


int solP_expr (parser_t *P)
{
  return solP_binary(P, 0);
}
