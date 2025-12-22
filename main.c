#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "lexer.h"
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

  lexer_t lex = LEXER_INIT;
  lex.src = buf;
  lex.len = len;

  parser_t parser = PARSER_INIT;
  uint64_t val = parse_int(&parser, &lex);
  printf("parsed: %lu\n", val);

  free(buf);
  return 0;
}
