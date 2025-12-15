/* utility */

#ifndef SOL_UTIL_H_
#define SOL_UTIL_H_

#include <stddef.h>

/**
 * read an ascii file from `path`, and set its length to `len` (if not null).
 * returns an allocated memory.
 */
char *readfile(char *path, size_t *len);

#endif /* SOL_UTIL_H_ */
