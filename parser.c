#include "parser.h"


void parser_init(parser_t *parser, char *src, int len)
{
  // hardcoded: enough to contain ast nodes of small srcs
  parser->arena = arena_init(8192);
  parser->lexer = LEXER_INIT;
  parser->lexer.src = src;
  parser->lexer.len = len;
  parser->err_cnt = 0;
}


void parser_free(parser_t *parser)
{
  arena_free(parser->arena);
  parser->arena = NULL;
  parser->lexer = LEXER_INIT;
}
