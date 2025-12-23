/* generates tokens from source */

#ifndef lexer_h
#define lexer_h 1

#include <stdlib.h> /* NULL */

typedef struct token token_t;
struct token {
  char *start;
  int len;
  int line;
  int col;
  int ln_off;
  int pos;
  int type;
};

typedef struct lexer lexer_t;
struct lexer {
  token_t curr, next;
  char *src;
  int len;
  int line;
  int col;
  int ln_off; /* actual how many chars from last LF */
  int pos;
};

/* token types */
#define T_INVALID         (-2)
#define T_UNKNOWN         (-1)
#define T_EOF              (0)
#define T_IDENTIFIER       (1)
#define T_INTEGER          (2)
#define T_PLUS             (3)   /* + */
#define T_DASH             (4)   /* - */
#define T_STAR             (5)   /* * */
#define T_SLASH            (6)   /* / */
#define T_LPAREN           (7)   /* ( */
#define T_RPAREN           (8)   /* ) */

#define TOKEN_INIT ((token_t){NULL,0,0,0,0,0,T_INVALID})
#define LEXER_INIT ((lexer_t){TOKEN_INIT,TOKEN_INIT,NULL,0,1,1,0,0})

/**
 * parse the next token.
 */
token_t lexer_tokenize(lexer_t *lex);

/**
 * returns the next token. steps the token stream
 */
token_t lexer_consume(lexer_t *lex);

/**
 * peek the next token but dont consume
 */
token_t lexer_peek(lexer_t *lex);

/**
 * returns the current token
 */
token_t lexer_current(lexer_t *lex);

#endif /* lexer_h */
