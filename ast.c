#include "ast.h"


struct binopinfo get_binopinfo(int op)
{
  static const struct binopinfo binops[] = {
    [AST_INVALID] = { -1, -1 },
    [BIN_ADD]     = { 1, LASSOC },
    [BIN_SUB]     = { 1, LASSOC },
    [BIN_MUL]     = { 2, LASSOC },
    [BIN_DIV]     = { 2, LASSOC },
  };
  static const int len = sizeof(binops)/sizeof(struct binopinfo);
  // bound check.
  if (op >= len)
    return binops[0];
  return binops[op];
}
