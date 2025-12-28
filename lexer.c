#include "lexer.h"
#include "char.h"

/**
 * increment the lexer
 */
static inline void increment (lexer_t *L, char ch)
{
  L->pos++;
  L->ln_off++;
  switch (ch) {
    case LF:
      L->line++;
      L->ln_off = 0;
      /* fallthrough */
    case CR:
      L->col = 1;
      break;
    case TAB:
      L->col += TABSTOP - ((L->col - 1) % TABSTOP);
      break;
    default:
      L->col++;
  }
}


/* Loop condition for consuming sequences of chars. */
#define for_ch(cond) (L->pos < L->len && (ch = L->src[L->pos], (cond)))


token_t solL_tokenize (lexer_t *L)
{
  char ch;
  while (1) {
    // skip initial whitespace
    while (for_ch(is_wspace(ch)))
      increment(L, ch);

    token_t tok = TOKEN_INIT;
    tok.pos = L->pos;
    tok.line = L->line;
    tok.col = L->col;
    tok.ln_off = L->ln_off;
    tok.start = &L->src[L->pos];

    // eof token
    if (L->pos >= L->len) {
      tok.type = T_EOF;
      tok.len = 0;
      return tok;
    }

    // identifier token
    if (is_ident(ch)) {
      while (for_ch(is_ident(ch) || is_digit(ch))) {
        increment(L, ch);
        tok.len++;
      } 
      tok.type = T_IDENTIFIER;
      return tok;
    }

    // integer token
    if (is_digit(ch)) {
      while (for_ch(is_digit(ch))) {
        increment(L, ch);
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
    increment(L, ch);
    tok.len++;
    return tok;

unknown:
    // unknown token
    while (for_ch(!is_wspace(ch))) {
      increment(L, ch);
      tok.len++;
    }
    tok.type = T_UNKNOWN;
    return tok;
  }
}


token_t solL_consume (lexer_t *L)
{
  if (L->next.type == T_INVALID)
    L->curr = solL_tokenize(L);
  else {
    L->curr = L->next;
    L->next.type = T_INVALID;
  }
  return L->curr;
}


token_t solL_peek (lexer_t *L)
{
  if (L->next.type == T_INVALID)
    L->next = solL_tokenize(L);
  return L->next;
}


token_t solL_current (lexer_t *L)
{
  if (L->curr.type == T_INVALID) {
    if (L->next.type == T_INVALID)
      L->curr = solL_tokenize(L);
    else {
      L->curr = L->next;
      L->next.type = T_INVALID;
    }
  }
  return L->curr;
}
