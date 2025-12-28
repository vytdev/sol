#include "solc.h"
#include "sol.h"
#include "parser.h"
#include "codegen.h"
#include "lexer.h"
#include <stdint.h>


// parse hierarchy:
// expr -> infix -> prefix -> postfix -> primary -> leaf


int solcP_reference (solc *C)
{
  token_t tok = solcL_consume(C);
  if (tok.type != T_IDENTIFIER)
    return solc_err(C, &tok, "syntax error: Expected identifier\n");
  // TODO: codegen
  return CSUCC;
}


int solcP_int (solc *C)
{
  token_t tok = solcL_consume(C);
  if (tok.type != T_INTEGER)
    return solc_err(C, &tok, "syntax error: Expected an integer\n");

  // simply parse a base 10 uint. we don't care about the sign here
  uint64_t val = 0;
  for (uint i = 0; i < tok.len; i++) {
    char digit = tok.start[i] - '0';
    // efficiently prevent overflow
    if (val > UINT64_MAX / 10 || (val == UINT64_MAX / 10
        && digit > UINT64_MAX % 10))
      return solc_err(C, &tok,
          "syntax error: Integer too big to fit into 64-bits\n");
    val = val * 10 + digit;
  }

  if (solcG_emitbyte(C, O_PUSH64) != CSUCC)
    return CFAIL;
  if (solcG_emit64(C, val) != CSUCC)
    return CFAIL;
  return CSUCC;
}


int solcP_primary (solc *C)
{
  token_t peek = solcL_peek(C);
  switch (peek.type) {
    case T_IDENTIFIER:
      return solcP_reference(C);
    case T_INTEGER:
      return solcP_int(C);
    case T_LPAREN: {
      solcL_consume(C);
      if (solcP_expr(C) != CSUCC)
        return CFAIL;
      peek = solcL_consume(C);
      if (peek.type != T_RPAREN)
        return solc_err(C, &peek, "syntax error: Expected ')' to close '('\n");
      return CSUCC;
    }
    default:
      return solc_err(C, &peek, "syntax error: Expected expression\n");
  }
}


/* unary operators */
#define UNR_UNK     (0)     /* ???? */
#define UNR_NEG     (1)     /* -v */
#define UNR_PLS     (2)     /* +v */

/* for converting unary op nums to inst opcode */
static const int unrop2opc[] = {
  [UNR_UNK] = O_NOP,
  [UNR_NEG] = O_NEG,
  [UNR_PLS] = O_NOP,    // doesn't map to any opcode
};

/*
 * get unrop (UNR_*) based on token type (T_*)
 */
static int tok2unrop (int ttype)
{
  switch (ttype) {
    case T_DASH:      return UNR_NEG;
    case T_PLUS:      return UNR_PLS;
    default:          return UNR_UNK;
  }
}


int solcP_postfix (solc *C)
{
  return solcP_primary(C);  // no postfix ops yet
}


int solcP_prefix (solc *C)
{
  token_t peek = solcL_peek(C);
  int op = tok2unrop(peek.type);
  if (op == UNR_UNK)
    return solcP_postfix(C);
  solcL_consume(C);   // consume op token

  // parse sub-unary exprs
  if (solcP_prefix(C) != CSUCC)
    return CFAIL;

  // plus prefix is basically a no-op
  if (op == UNR_PLS)
    return CSUCC;

  if (solcG_emitbyte(C, unrop2opc[op]) != CSUCC)
    return CFAIL;
  return CSUCC;
}


/* binary operators */
#define BIN_UNK       (0)     /* what? */
#define BIN_ADD       (1)     /* + */
#define BIN_SUB       (2)     /* - */
#define BIN_MUL       (3)     /* * */
#define BIN_DIV       (4)     /* / */

/* for converting binary op nums to inst opcode */
static const int binop2opc[] = {
  [BIN_UNK] = O_NOP,
  [BIN_ADD] = O_ADD,
  [BIN_SUB] = O_SUB,
  [BIN_MUL] = O_MUL,
  [BIN_DIV] = O_DIV,
};

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


int solcP_infix (solc *C, int min_prec)
{
  // parse initial lhs
  if (solcP_prefix(C) != CSUCC)
    return CFAIL;

  for (;;) {
    // check if next token is a binary op.
    token_t peek = solcL_peek(C);
    int op = tok2binop(peek.type);
    if (op == BIN_UNK)
      break;

    // get binopinfo. check if prec is too low than min.
    const struct binopinfo *info = &binops[op];
    if (info->prec < min_prec)
      break;
    solcL_consume(C);   // consume op.

    // parse rhs
    if (solcP_infix(C, info->prec + info->assoc) != CSUCC)
      return CFAIL;

    // the op instruction
    if (solcG_emitbyte(C, binop2opc[op]) != CSUCC)
      return CFAIL;
  }

  return CSUCC;
}


int solcP_expr (solc *C)
{
  return solcP_infix(C, 0);
}
