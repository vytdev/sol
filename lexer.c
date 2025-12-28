#include "lexer.h"
#include "char.h"

/**
 * increment the lexer
 */
static inline void increment (solc *C, char ch)
{
  C->pos++;
  C->ln_off++;
  switch (ch) {
    case LF:
      C->line++;
      C->ln_off = 0;
      /* fallthrough */
    case CR:
      C->col = 1;
      break;
    case TAB:
      C->col += TABSTOP - ((C->col - 1) % TABSTOP);
      break;
    default:
      C->col++;
  }
}


/* Loop condition for consuming sequences of chars. */
#define for_ch(cond) (C->pos < C->len && (ch = C->src[C->pos], (cond)))


token_t solL_tokenize (solc *C)
{
  char ch;
  while (1) {
    // skip initial whitespace
    while (for_ch(is_wspace(ch)))
      increment(C, ch);

    token_t tok = TOKEN_INIT;
    tok.pos = C->pos;
    tok.line = C->line;
    tok.col = C->col;
    tok.ln_off = C->ln_off;
    tok.start = &C->src[C->pos];

    // eof token
    if (C->pos >= C->len) {
      tok.type = T_EOF;
      tok.len = 0;
      return tok;
    }

    // identifier token
    if (is_ident(ch)) {
      while (for_ch(is_ident(ch) || is_digit(ch))) {
        increment(C, ch);
        tok.len++;
      } 
      tok.type = T_IDENTIFIER;
      return tok;
    }

    // integer token
    if (is_digit(ch)) {
      while (for_ch(is_digit(ch))) {
        increment(C, ch);
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
    increment(C, ch);
    tok.len++;
    return tok;

unknown:
    // unknown token
    while (for_ch(!is_wspace(ch))) {
      increment(C, ch);
      tok.len++;
    }
    tok.type = T_UNKNOWN;
    return tok;
  }
}


token_t solL_consume (solc *C)
{
  if (C->next.type == T_INVALID)
    C->curr = solL_tokenize(C);
  else {
    C->curr = C->next;
    C->next.type = T_INVALID;
  }
  return C->curr;
}


token_t solL_peek (solc *C)
{
  if (C->next.type == T_INVALID)
    C->next = solL_tokenize(C);
  return C->next;
}


token_t solL_current (solc *C)
{
  if (C->curr.type == T_INVALID) {
    if (C->next.type == T_INVALID)
      C->curr = solL_tokenize(C);
    else {
      C->curr = C->next;
      C->next.type = T_INVALID;
    }
  }
  return C->curr;
}
