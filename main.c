#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "parser.h"


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

  parser_t parser;
  parser_init(&parser, buf, len);
  int err = parse_expr(&parser);
  printf("err cnt: %d\n", err);

  parser_free(&parser);
  free(buf);
  return 0;
}
