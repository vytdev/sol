/* custom arena allocator */

#ifndef arena_h
#define arena_h 1

#include <stddef.h>


typedef struct arena arena_t;
struct arena {
  arena_t *next;
  size_t pos;
  size_t alloc;
  char buf[];
};

/**
 * creates a new arena with initial size.
 */
arena_t *arena_init(size_t size);

/**
 * allocate from arena.
 */
void *arena_new(arena_t *arena, size_t size);

/**
 * free up the given arena.
 */
void arena_free(arena_t *arena);

#endif /* arena_h */
