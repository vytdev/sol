#include "util.h"
#include "char.h" /* TABSTOP */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>


char *readfile (char *path, size_t *len)
{
  FILE *fp = fopen(path, "rb");
  if (!fp)
    return NULL;

  fseek(fp, 0, SEEK_END);
  size_t sz = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (sz == 0) {
    fclose(fp);
    return NULL;
  }

  char *buf = (char*)malloc(sz+1);
  if (!buf) {
    fclose(fp);
    return NULL;
  }
  buf[sz] = '\0';

  size_t in = fread(buf, 1, sz, fp);
  if (in != sz) {
    fclose(fp);
    free(buf);
    return NULL;
  }

  fclose(fp);
  if (len)
    *len = sz;
  return buf;
}


void print_token (token_t *tok, int len)
{
  // don't print invalid tokens
  if (tok->type == T_INVALID)
    return;

  // print the line text
  printf(" %5d| ", tok->line);
  for (int i = -tok->ln_off, tabcol = 0; tok->pos + i < len; i++) {
    char ch = tok->start[i];
    if (ch == CR || ch == LF)
      break;
    if (ch == TAB) {
      int tabwidth = TABSTOP - (tabcol % TABSTOP);
      for (int j = 0; j < tabwidth; j++) fputc(' ', stdout);
      tabcol += tabwidth;
      continue;
    }
    fputc(ch, stdout);
    tabcol++;
  }

  // the caret line
  printf("\n      | ");
  for (int i = 1; i < tok->col; i++) fputc(' ', stdout);
  for (int i = 0; i < tok->len; i++) fputc('^', stdout);

  // eof has length 0, so instead carets, we'll print "eof"
  if (tok->type == T_EOF)
    printf("eof");

  fputc('\n', stdout);
}


void msgtok (token_t *tok, lexer_t *lex, char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  print_token(tok, lex->len);
}


NORETRN void fatal_err (char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  // we're a standalone executable with no sensitive open databases.
  // we don't have to free everything manually.
  exit(255);
}
