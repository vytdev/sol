#include "ast.h"
#include "lexer.h"
#include <stdio.h>


struct binopinfo ast_get_binopinfo(int op)
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


int ast_tok2binop(int tok)
{
  switch (tok) {
    case T_PLUS:        return BIN_ADD;
    case T_DASH:        return BIN_SUB;
    case T_STAR:        return BIN_MUL;
    case T_SLASH:       return BIN_DIV;
    default:            return AST_INVALID;
  }
}


static void ast_print_expr_1(ast_expr *expr)
{
  if (!expr) {
    printf("null");
    return;
  }
  switch (expr->type) {
    case EXPR_INT:
      printf("%lu", expr->uint.val);
      break;
    case EXPR_BINARY:
      fputc('(', stdout);
      ast_print_expr_1(expr->bin.lhs);
      printf(" %c ", expr->tok.start[0]);
      ast_print_expr_1(expr->bin.rhs);
      fputc(')', stdout);
      break;
    default:
      printf("unknown");
  }
}


void ast_print_expr(ast_expr *expr)
{
  ast_print_expr_1(expr);
  fputc('\n', stdout);
}
