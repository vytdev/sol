/* front end */

#ifndef solc_h
#define solc_h 1

#include <stdint.h>


typedef unsigned long ulong;
typedef unsigned int uint;

typedef struct {
  char *start;
  uint len;
  uint line;
  uint col;
  uint pos;
  uint ln_off;
  int type;
} token_t;

typedef struct {
  char *msg;
  token_t token;
} compile_err;

typedef struct solc {
  /* lexer ctx */
  token_t curr, next;
  char *src;
  uint len, line, col, pos, ln_off;
  /* compile errors */
  compile_err *err;
  int err_cnt, err_max;
  /* codegen */
  char *code;
  ulong cpos, clen;
} solc;

/* compiler states */
#define CSUCC  (0)
#define CFAIL  (1)

/* funcs defined in: compile.c */

/**
 * initialize a compiler context.
 */
void solc_init (solc *C, char *src, int len, char *code, ulong clen);

/**
 * set error msg buffer.
 */
void solc_seterrbuf (solc *C, compile_err *buf, int len);

/**
 * add a compile error.
 */
int solc_err (solc *C, token_t *tok, char *msg);

/**
 * compile source into bytecode. returns the number of errors.
 */
int solc_compile (solc *C);

#endif /* solc_h */
