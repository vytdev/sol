#include "compile.h"
#include "parser.h"
#include "vm.h"


void solC_init (compiler_t *C, char *src, int len, char *code, ulong clen)
{
  C->lex = LEXER_INIT;
  C->lex.src = src;
  C->lex.len = len;
  // error reporting
  C->err = NULL;
  C->err_cnt = 0;
  C->err_max = 0;
  // for codegen
  C->code = code;
  C->cpos = 0;
  C->clen = clen;
}


void solC_seterrbuf (compiler_t *C, struct compile_err *buf, int len)
{
  C->err = buf;
  C->err_max = len;
  C->err_cnt = 0;
}


int solC_err (compiler_t *C, token_t *tok, char *msg)
{
  if (C->err_cnt < C->err_max) {
    C->err[C->err_cnt].msg = msg;
    C->err[C->err_cnt].token = tok ? *tok : TOKEN_INIT;
  }
  C->err_cnt++;
  return C->err_cnt;
}


int solC_compile (compiler_t *C)
{
  // could change in the future
  int err = solP_expr(C);
  if (err) return err;
  return solC_emitbyte(C, O_HALT);
}


/* bin op to opcode map */
const int solC_binop2opc[BIN_LENGTH] = {
  [BIN_UNK] = O_NOP,
  [BIN_ADD] = O_ADD,
  [BIN_SUB] = O_SUB,
  [BIN_MUL] = O_MUL,
  [BIN_DIV] = O_DIV,
};


int solC_emit (compiler_t *C, char *bytes, ulong cnt)
{
  if (C->err_cnt > 0)
    return C->err_cnt;
  if (C->cpos + cnt > C->clen)
    return solC_err(C, NULL, "code gen: Ran out of memory\n");
  for (ulong i = 0; i < cnt; i++)
    C->code[C->cpos + i] = bytes[i];
  C->cpos += cnt;
  return CSUCC;
}


int solC_emitbyte (compiler_t *C, char byte)
{
  if (C->err_cnt > 0)
    return C->err_cnt;
  if (C->cpos + 1 > C->clen)
    return solC_err(C, NULL, "code gen: Ran out of memory\n");
  C->code[C->cpos++] = byte;
  return CSUCC;
}


int solC_emit64 (compiler_t *C, uint64_t val)
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
  return solC_emit(C, buf, 8);
}
