#include <stdio.h>
#include <stdlib.h>
#include "util.h"


int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("usage: %s FILE\n", argv[0]);
    return 1;
  }

  size_t len = 0;
  char *buf = readfile(argv[1], &len);
  if (!buf) {
    fprintf(stderr, "%s: Failed to open file: %s\n", argv[0], argv[1]);
    return 1;
  }

  fwrite(buf, 1, len, stdout);
  free(buf);
  return 0;
}
