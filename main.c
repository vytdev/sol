#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "parser.h"


int main (int argc, char **argv)
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
  solP_init(&parser, buf, len);
  int err = solP_expr(&parser);
  for (int i = 0; i < err; i++)
    msgtok(&parser.err[i].token, &parser.lex, parser.err[i].msg);
  printf("err cnt: %d\n", err);

  solP_free(&parser);
  free(buf);
  return 0;
}
