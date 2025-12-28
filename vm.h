/* vm component of sol */

#ifndef sol_vm_h
#define sol_vm_h 1

#include <stdint.h>


typedef unsigned long ulong;
typedef struct sol sol;

/* vm opcodes */
#define O_NOP       (0)     /* no-op */
#define O_HALT      (1)     /* pause vm */
#define O_ADD       (2)     /* binary add */
#define O_SUB       (3)     /* binary sub */
#define O_MUL       (4)     /* binary mul */
#define O_DIV       (5)     /* binary div */
#define O_PUSH64    (6)     /* push 64-bit int */
#define O_POP       (7)     /* pop the stack */

/* value type */
typedef union {
  int64_t intv;
} value_t;

#define TV_NULL      (0)
#define TV_INT       (1)

typedef struct {
  int type;
  value_t v;
} tvalue_t;


/* vm states (return of sol_exec) */
#define S_OK          (0)
#define S_ERROR       (1)     /* generic error */
#define S_EINST       (2)     /* illegal instruction */
#define S_ESTACK      (3)     /* stack overflow */

/* maximum length of value stack (tmp) */
#define MAXSTACK 8192

struct sol {
  char *code;
  ulong clen;
  ulong pc;
  /* value stack */
  tvalue_t stack[MAXSTACK];
  ulong stpos;
  ulong stlen;
};

/*
 * initialize a sol state.
 */
void sol_init (sol *T);

/*
 * push a tvalue onto the stack.
 */
int sol_push (sol *T, tvalue_t val);

/*
 * pop a tvalue from the stack.
 */
int sol_pop (sol *T, tvalue_t *val);

/*
 * execute the given state until halt.
 */
int sol_exec (sol *T);

#endif /* sol_vm_h */
