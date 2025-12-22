/* utility */

#ifndef UTIL_H_
#define UTIL_H_ 1

#include <stddef.h>

/**
 * read an ascii file from `path`, and set its length to `len` (if not null).
 * returns an allocated memory.
 */
char *readfile(char *path, size_t *len);

#endif /* UTIL_H_ */
