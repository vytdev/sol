/* ast tree definitions */

#ifndef AST_H_
#define AST_H_ 1

#include <stdint.h>
#include "lexer.h"


/* ast node types. */

#define AST_INVALID       (0)

#define EXPR_INT          (1)
#define EXPR_BINARY       (2)

#define BIN_ADD           (1)       /* + */
#define BIN_SUB           (2)       /* - */
#define BIN_MUL           (3)       /* * */
#define BIN_DIV           (4)       /* / */

/* ast struct alias definitions. */
typedef struct ast_int ast_int;
typedef struct ast_binary ast_binary;
typedef struct ast_expr ast_expr;

struct ast_int {
  uint64_t val;
};

struct ast_binary {
  int op;
  ast_expr *lhs;
  ast_expr *rhs;
};

struct ast_expr {
  token_t tok;
  int type;
  union {
    ast_int     uint;
    ast_binary  bin;
  };
};

/* binary op info. */
#define LASSOC  (1)
#define RASSOC  (0)
struct binopinfo {
  int prec;
  int assoc;
};

/**
 * returns the binopinfo of the given binary operator (BIN_*).
 */
struct binopinfo ast_get_binopinfo(int op);

/**
 * returns the binary operator (BIN_*) type from token type (T_*). AST_INVALID
 * if not found.
 */
int ast_tok2binop(int tok);

/**
 * debugging utility to print an ast_expr tree.
 */
void ast_print_expr(ast_expr *expr);

#endif /* AST_H_ */
