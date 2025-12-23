#include "parser.h"
#include "util.h"


ast_expr *parse_reference(parser_t *parser)
{
  token_t tok = lexer_consume(&parser->lexer);
  if (tok.type != T_IDENTIFIER) {
    msgtok(&tok, &parser->lexer, "syntax error: Expected identifier\n");
    parser->err_cnt++;
    return NULL;
  }
  // new node
  ast_expr *expr = newnode(ast_expr);
  expr->tok = tok;
  expr->type = EXPR_REF;
  return expr;
}


ast_expr *parse_int(parser_t *parser)
{
  token_t tok = lexer_consume(&parser->lexer);
  if (tok.type != T_INTEGER) {
    msgtok(&tok, &parser->lexer, "syntax error: Expected an integer\n");
    parser->err_cnt++;
    return NULL;
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
      parser->err_cnt++;
      return NULL;
    }
    val = val * 10 + digit;
  }

  // new node
  ast_expr *expr = newnode(ast_expr);
  expr->tok = tok;
  expr->type = EXPR_INT;
  expr->uint.val = val;
  return expr;
}


ast_expr *parse_primary(parser_t *parser)
{
  token_t peek = lexer_peek(&parser->lexer);
  switch (peek.type) {
    case T_IDENTIFIER:
      return parse_reference(parser);
    case T_INTEGER:
      return parse_int(parser);
    case T_LPAREN: {
      lexer_consume(&parser->lexer);
      ast_expr *expr = parse_expr(parser);
      if (!expr) return NULL;
      peek = lexer_consume(&parser->lexer);
      if (peek.type != T_RPAREN) {
        msgtok(&peek, &parser->lexer,
               "syntax error: Expected ')' to close '('\n");
        parser->err_cnt++;
        return NULL;
      }
      return expr;
    }
    default:
      msgtok(&peek, &parser->lexer, "syntax error: Expected expression\n");
      parser->err_cnt++;
      return NULL;
  }
}


ast_expr *parse_binary(parser_t *parser, int min_prec)
{
  ast_expr *lhs = parse_primary(parser);
  if (!lhs) return NULL;

  for (;;) {
    // check if next token is a binary op.
    token_t peek = lexer_peek(&parser->lexer);
    int op = ast_tok2binop(peek.type);
    if (op == AST_INVALID)
      break;

    // get binopinfo. check if prec is too low than min.
    struct binopinfo info = ast_get_binopinfo(op);
    if (info.prec < min_prec)
      break;
    lexer_consume(&parser->lexer);   // consume op.

    ast_expr *rhs = parse_binary(parser, info.prec + info.assoc);
    if (!rhs) return NULL;

    // setup binary op node.
    ast_expr *binop = newnode(ast_expr);
    binop->type = EXPR_BINARY;
    binop->tok = peek;
    binop->bin.lhs = lhs;
    binop->bin.rhs = rhs;

    lhs = binop;
  }

  return lhs;
}


ast_expr *parse_expr(parser_t *parser)
{
  return parse_binary(parser, 0);
}
