#ifndef SOL_LEXER_H_
#define SOL_LEXER_H_

#include <stdlib.h> /* NULL */

struct Token {
  char *start;
  int len;
  int line;
  int col;
  int pos;
  int type;
};

struct Lexer {
  struct Token curr, next;
  char *src;
  int len;
  int line;
  int col;
  int pos;
};

/* token types */
#define T_INVALID         (-2)
#define T_UNKNOWN         (-1)
#define T_EOF              (0)
#define T_IDENTIFIER       (1)
#define T_NUMBER           (2)
#define T_PLUS             (3)   /* + */
#define T_DASH             (4)   /* - */
#define T_STAR             (5)   /* * */
#define T_SLASH            (6)   /* / */
#define T_LPAREN           (7)   /* ( */
#define T_RPAREN           (8)   /* ) */

#define TOKEN_INIT ((struct Token){NULL,0,0,0,0,T_INVALID})
#define LEXER_INIT ((struct Lexer){TOKEN_INIT,TOKEN_INIT,NULL,0,1,1,0})

/**
 * parse the next token.
 */
struct Token solL_tokenize(struct Lexer *lex);

/**
 * returns the next token. steps the token stream
 */
struct Token solL_consume(struct Lexer *lex);

/**
 * peek the next token but dont consume
 */
struct Token solL_peek(struct Lexer *lex);

/**
 * returns the current token
 */
struct Token solL_current(struct Lexer *lex);

#endif /* SOL_LEXER_H_ */
