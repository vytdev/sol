/* vm component of sol */

#ifndef sol_h
#define sol_h 1

#include <stdint.h>


typedef unsigned long ulong;
typedef struct sol sol;

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
void sol_init (sol *T, char *code, ulong clen);

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

#endif /* sol_h */
