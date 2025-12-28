/* generates tokens from source */

#ifndef sol_lexer_h
#define sol_lexer_h 1

#include "compile.h"


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

#define TOKEN_INIT ((token_t){0,0,0,0,0,0,T_INVALID})

/**
 * parse the next token.
 */
token_t solcL_tokenize (solc *C);

/**
 * returns the next token. steps the token stream
 */
token_t solcL_consume (solc *C);

/**
 * peek the next token but dont consume
 */
token_t solcL_peek (solc *C);

/**
 * returns the current token
 */
token_t solcL_current (solc *C);

#endif /* sol_lexer_h */
