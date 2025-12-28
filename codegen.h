/* codegen */

#ifndef solcG_h
#define solcG_h 1

#include "solc.h"


/* funcs defined in: compile.c */

/**
 * emit bytes to code buffer.
 */
int solcG_emit (solc *C, char *bytes, ulong cnt);

/**
 * emit a single byte.
 */
int solcG_emitbyte (solc *C, char byte);

/**
 * emit a 64-bit value.
 */
int solcG_emit64 (solc *C, uint64_t val);

#endif /* solcG_h */
