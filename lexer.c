#include "lexer.h"
#include "char.h"

/**
 * increment the lexer
 */
static inline void solL_increment(struct Lexer *lex, char ch)
{
  lex->pos++;
  switch (ch) {
    case LF:
      lex->col = 1;
      /* fallthrough */
    case CR:
      lex->line++;
      break;
    case TAB:
      lex->col += TABSTOP - (lex->col % TABSTOP);
      break;
    default:
      lex->col++;
  }
}


struct Token solL_tokenize(struct Lexer *lex)
{
  char ch;
  while (1) {
    // skip initial whitespace
    while (lex->pos < lex->len && (ch = lex->src[lex->pos], is_wspace(ch)))
      solL_increment(lex, ch);

    struct Token tok = TOKEN_INIT;
    tok.pos = lex->pos;
    tok.line = lex->line;
    tok.col = lex->col;
    tok.start = &lex->src[lex->pos];

    // eof token
    if (lex->pos >= lex->len) {
      tok.type = T_EOF;
      tok.len = 0;
      return tok;
    }

    // unknown token
    while (lex->pos < lex->len && (ch = lex->src[lex->pos], !is_wspace(ch))) {
      solL_increment(lex, ch);
      tok.len++;
    }
    tok.type = T_UNKNOWN;
    return tok;
  }
}


struct Token solL_consume(struct Lexer *lex)
{
  if (lex->next.type == T_INVALID)
    lex->curr = solL_tokenize(lex);
  else {
    lex->curr = lex->next;
    lex->next.type = T_INVALID;
  }
  return lex->curr;
}


struct Token solL_peek(struct Lexer *lex)
{
  if (lex->next.type == T_INVALID)
    lex->next = solL_tokenize(lex);
  return lex->next;
}


struct Token solL_current(struct Lexer *lex)
{
  if (lex->curr.type == T_INVALID) {
    if (lex->next.type == T_INVALID)
      lex->curr = solL_tokenize(lex);
    else {
      lex->curr = lex->next;
      lex->next.type = T_INVALID;
    }
  }
  return lex->curr;
}
