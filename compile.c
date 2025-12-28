#include "compile.h"
#include "parser.h"
#include "vm.h"
#include "lexer.h"
#include <stdlib.h>  // for NULL.


void solc_init (solc *C, char *src, int len, char *code, ulong clen)
{
  // lexing
  C->curr = TOKEN_INIT;
  C->next = TOKEN_INIT;
  C->src = src;
  C->len = len;
  C->line = 1;
  C->col = 1;
  C->pos = 0;
  C->ln_off = 0;
  // error reporting
  C->err = NULL;
  C->err_cnt = 0;
  C->err_max = 0;
  // for codegen
  C->code = code;
  C->cpos = 0;
  C->clen = clen;
}


void solc_seterrbuf (solc *C, compile_err *buf, int len)
{
  C->err = buf;
  C->err_max = len;
  C->err_cnt = 0;
}


int solc_err (solc *C, token_t *tok, char *msg)
{
  if (C->err_cnt < C->err_max) {
    C->err[C->err_cnt].msg = msg;
    C->err[C->err_cnt].token = tok ? *tok : TOKEN_INIT;
  }
  C->err_cnt++;
  return C->err_cnt;
}


int solc_compile (solc *C)
{
  // could change in the future
  int err = solcP_expr(C);
  if (err) return err;
  return solcG_emitbyte(C, O_HALT);
}


int solcG_emit (solc *C, char *bytes, ulong cnt)
{
  if (C->err_cnt > 0)
    return C->err_cnt;
  if (C->cpos + cnt > C->clen)
    return solc_err(C, NULL, "code gen: Ran out of memory\n");
  for (ulong i = 0; i < cnt; i++)
    C->code[C->cpos + i] = bytes[i];
  C->cpos += cnt;
  return CSUCC;
}


int solcG_emitbyte (solc *C, char byte)
{
  if (C->err_cnt > 0)
    return C->err_cnt;
  if (C->cpos + 1 > C->clen)
    return solc_err(C, NULL, "code gen: Ran out of memory\n");
  C->code[C->cpos++] = byte;
  return CSUCC;
}


int solcG_emit64 (solc *C, uint64_t val)
{
  char buf[8];
  buf[0] = (val >>  0) & 0xff;
  buf[1] = (val >>  8) & 0xff;
  buf[2] = (val >> 16) & 0xff;
  buf[3] = (val >> 24) & 0xff;
  buf[4] = (val >> 32) & 0xff;
  buf[5] = (val >> 40) & 0xff;
  buf[6] = (val >> 48) & 0xff;
  buf[7] = (val >> 56) & 0xff;
  return solcG_emit(C, buf, 8);
}
