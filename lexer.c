#include "lexer.h"
#include "char.h"

/**
 * increment the lexer
 */
static inline void lexer_increment(lexer_t *lex, char ch)
{
  lex->pos++;
  switch (ch) {
    case LF:
      lex->line++;
      /* fallthrough */
    case CR:
      lex->col = 1;
      break;
    case TAB:
      lex->col += TABSTOP - (lex->col % TABSTOP);
      break;
    default:
      lex->col++;
  }
}


/* Loop condition for consuming sequences of chars. */
#define for_ch(cond) (lex->pos < lex->len && (ch = lex->src[lex->pos], (cond)))


token_t lexer_tokenize(lexer_t *lex)
{
  char ch;
  while (1) {
    // skip initial whitespace
    while (for_ch(is_wspace(ch)))
      lexer_increment(lex, ch);

    token_t tok = TOKEN_INIT;
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

    // identifier token
    if (is_ident(ch)) {
      while (for_ch(is_ident(ch) || is_digit(ch))) {
        lexer_increment(lex, ch);
        tok.len++;
      } 
      tok.type = T_IDENTIFIER;
      return tok;
    }

    // integer token
    if (is_digit(ch)) {
      while (for_ch(is_digit(ch))) {
        lexer_increment(lex, ch);
        tok.len++;
      }
      tok.type = T_INTEGER;
      return tok;
    }

    // single char tokens
    switch (ch) {
      case '+': tok.type = T_PLUS;    break;
      case '-': tok.type = T_DASH;    break;
      case '*': tok.type = T_STAR;    break;
      case '/': tok.type = T_SLASH;   break;
      case '(': tok.type = T_LPAREN;  break;
      case ')': tok.type = T_RPAREN;  break;
      default: goto unknown;
    }
    lexer_increment(lex, ch);
    tok.len++;
    return tok;

unknown:
    // unknown token
    while (for_ch(!is_wspace(ch))) {
      lexer_increment(lex, ch);
      tok.len++;
    }
    tok.type = T_UNKNOWN;
    return tok;
  }
}


token_t lexer_consume(lexer_t *lex)
{
  if (lex->next.type == T_INVALID)
    lex->curr = lexer_tokenize(lex);
  else {
    lex->curr = lex->next;
    lex->next.type = T_INVALID;
  }
  return lex->curr;
}


token_t lexer_peek(lexer_t *lex)
{
  if (lex->next.type == T_INVALID)
    lex->next = lexer_tokenize(lex);
  return lex->next;
}


token_t lexer_current(lexer_t *lex)
{
  if (lex->curr.type == T_INVALID) {
    if (lex->next.type == T_INVALID)
      lex->curr = lexer_tokenize(lex);
    else {
      lex->curr = lex->next;
      lex->next.type = T_INVALID;
    }
  }
  return lex->curr;
}
