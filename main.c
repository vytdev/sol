#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "lexer.h"


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

  struct Lexer lex = LEXER_INIT;
  lex.src = buf;
  lex.len = len;

  // print tokens (temporary)
  struct Token tok;
  do {
    tok = solL_consume(&lex);
    printf("ty%d ln%d col%d: ", tok.type, tok.line, tok.col);
    for (int i = 0; i < tok.len; i++) fputc(tok.start[i], stdout);
    fputc('\n', stdout);
  } while (tok.type != T_EOF);

  free(buf);
  return 0;
}
