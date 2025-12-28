#include "parser.h"


void solP_init (parser_t *P, char *src, int len)
{
  P->lex = LEXER_INIT;
  P->lex.src = src;
  P->lex.len = len;
}


void solP_free (parser_t *P)
{
  P->lex = LEXER_INIT;
}


int solP_err (parser_t *P, int cnt, token_t *tok, char *msg)
{
  if (cnt < MAXPARSEERR) {
    P->err[cnt].msg = msg;
    P->err[cnt].token = *tok;
  }
  return cnt + 1;
}
