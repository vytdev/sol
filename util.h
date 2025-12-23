/* utility */

#ifndef sol_util_h
#define sol_util_h 1

#include "lexer.h"
#include <stddef.h>


/* no return attribute. */
#if defined(__GNUC__) || defined(__clang__)
#define NORETRN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define NORETRN __declspec(noreturn)
#else
#define NORETRN
#endif

/* align an offset to the next 8-byte boundary. */
#define ALIGN8(n) (((n) + 7) & ~7)

/**
 * read an ascii file from `path`, and set its length to `len` (if not null).
 * returns an allocated memory.
 */
char *readfile(char *path, size_t *len);

/**
 * pretty print a token, given the source length for bound checking
 */
void print_token(token_t *tok, int len);

/**
 * print a message with token.
 */
void msgtok(token_t *tok, lexer_t *lex, char *fmt, ...);

/**
 * print a fatal error and exit.
 */
NORETRN void fatal_err(char *fmt, ...);

#endif /* sol_util_h */
