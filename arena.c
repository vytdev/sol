#include "arena.h"
#include "util.h"
#include <stdlib.h>


arena_t *arena_init(size_t size)
{
  size = ALIGN8(size);
  arena_t *arena = (arena_t*)malloc(sizeof(arena_t) + size);
  if (!arena) fatal_err("error: Out of memory\n");
  arena->alloc = size;
  arena->pos = 0;
  arena->next = NULL;
  return arena;
}


void *arena_new(arena_t *arena, size_t size)
{
  size = ALIGN8(size);

  // try next arena.
  if (arena->alloc - arena->pos < size) {
    if (!arena->next)
      arena->next = arena_init(arena->alloc * 2 + size);
    return arena_new(arena->next, size);
  }

  // available mem found. allocate from there.
  void *mem = &arena->buf[arena->pos];
  arena->pos += size;
  return mem;
}


void arena_free(arena_t *arena)
{
  if (!arena)
    return;
  arena_free(arena->next);
  free(arena);
}
