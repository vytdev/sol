/* front end */

#ifndef sol_compile_h
#define sol_compile_h 1

#include "lexer.h"
#include <stdint.h>


typedef unsigned long ulong;

struct compile_err {
  char *msg;
  token_t token;
};

typedef struct compiler compiler_t;
struct compiler {
  lexer_t lex;
  /* compile errors */
  struct compile_err *err;
  int err_cnt, err_max;
  /* codegen */
  char *code;
  ulong cpos, clen;
};

/* compiler states */
#define CSUCC  (0)
#define CFAIL  (1)

/* binary operators */
#define BIN_UNK       (0)     /* what? */
#define BIN_ADD       (1)     /* + */
#define BIN_SUB       (2)     /* - */
#define BIN_MUL       (3)     /* * */
#define BIN_DIV       (4)     /* / */
#define BIN_LENGTH    (5)     /* num of bin ops */

/* for converting binary op nums to inst opcode */
extern const int solC_binop2opc[BIN_LENGTH];

/**
 * initialize a compiler context.
 */
void solC_init (compiler_t *C, char *src, int len, char *code, ulong clen);

/**
 * set error msg buffer.
 */
void solC_seterrbuf (compiler_t *C, struct compile_err *buf, int len);

/**
 * add a compile error.
 */
int solC_err (compiler_t *C, token_t *tok, char *msg);

/**
 * compile source into bytecode.
 */
int solC_compile (compiler_t *C);

/**
 * emit bytes to code buffer.
 */
int solC_emit (compiler_t *C, char *bytes, ulong cnt);

/**
 * emit a single byte.
 */
int solC_emitbyte (compiler_t *C, char byte);

/**
 * emit a 64-bit value.
 */
int solC_emit64 (compiler_t *C, uint64_t val);

#endif /* sol_compile_h */
