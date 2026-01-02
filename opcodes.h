/* vm opcodes */

#ifndef sol_opcodes_h
#define sol_opcodes_h 1

#define O_NOP       (0)     /* no-op */
#define O_HALT      (1)     /* pause vm */
#define O_ADD       (2)     /* binary add */
#define O_SUB       (3)     /* binary sub */
#define O_MUL       (4)     /* binary mul */
#define O_DIV       (5)     /* binary div */
#define O_PUSH64    (6)     /* push 64-bit int */
#define O_POP       (7)     /* pop the stack */
#define O_NEG       (8)     /* unary negate */

#endif /* sol_opcodes_h */
