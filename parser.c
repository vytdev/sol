#include "parser.h"


void parser_init(parser_t *parser, char *src, int len)
{
  parser->lexer = LEXER_INIT;
  parser->lexer.src = src;
  parser->lexer.len = len;
}


void parser_free(parser_t *parser)
{
  parser->lexer = LEXER_INIT;
}
