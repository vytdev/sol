#include "vm.h"
#include <stdlib.h>   /* for NULL. */


void sol_init (sol *T, char *code, ulong clen)
{
  T->code = code;
  T->clen = clen;
  T->pc = 0;
  // temporary
  T->stlen = MAXSTACK;
  T->stpos = 0;
}


int sol_push (sol *T, tvalue_t val)
{
  if (T->stlen <= T->stpos)
    return -S_ESTACK;
  T->stack[T->stpos++] = val;
  return S_OK;
}


int sol_pop (sol *T, tvalue_t *val)
{
  if (T->stpos == 0)
    return -S_ESTACK;
  T->stpos--;
  if (val)
    *val = T->stack[T->stpos];
  return S_OK;
}


#define binary_int_op(op) do { \
  tvalue_t a, b; int err; \
  if ((err = sol_pop(T, &b)) != S_OK) return err; \
  if ((err = sol_pop(T, &a)) != S_OK) return err; \
  a.v.intv = a.v.intv op b.v.intv; \
  if ((err = sol_push(T, a)) != S_OK) return err; \
} while (0)

#define unary_int_op(op) do { \
  tvalue_t tv; int err; \
  if ((err = sol_pop(T, &tv)) != S_OK) return err; \
  tv.v.intv = op tv.v.intv; \
  if ((err = sol_push(T, tv)) != S_OK) return err; \
} while (0)


int sol_exec (sol *T)
{
  const char *code = T->code;
  const ulong clen = T->clen;
  ulong pc = T->pc;

begin:
  if (pc >= clen)
    return -S_EINST;
  switch (code[pc++]) {
    case O_NOP:
      break;
    case O_HALT:
      return S_OK;
    case O_ADD:
      binary_int_op(+);
      break;
    case O_SUB:
      binary_int_op(-);
      break;
    case O_MUL:
      binary_int_op(*);
      break;
    case O_DIV:
      binary_int_op(/);
      break;
    case O_PUSH64: {
      if (pc + 8 >= clen)
        return -S_EINST;
      tvalue_t tv;
      tv.type = TV_INT;
      tv.v.intv =
        ((uint64_t)code[pc+0] <<  0) |
        ((uint64_t)code[pc+1] <<  8) |
        ((uint64_t)code[pc+2] << 16) |
        ((uint64_t)code[pc+3] << 24) |
        ((uint64_t)code[pc+4] << 32) |
        ((uint64_t)code[pc+5] << 40) |
        ((uint64_t)code[pc+6] << 48) |
        ((uint64_t)code[pc+7] << 56);
      sol_push(T, tv);
      pc += 8;
      break;
    }
    case O_POP:
      sol_pop(T, NULL);
      break;
    case O_NEG:
      unary_int_op(-);
      break;
    default:
      return -S_EINST;
  }

  goto begin;
}
