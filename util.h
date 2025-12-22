/* utility */

#ifndef UTIL_H_
#define UTIL_H_ 1

#include "lexer.h"
#include <stddef.h>

/**
 * read an ascii file from `path`, and set its length to `len` (if not null).
 * returns an allocated memory.
 */
char *readfile(char *path, size_t *len);

/**
 * pretty print a token, given the source length for bound checking
 */
void print_token(token_t *tok, int len);

#endif /* UTIL_H_ */
