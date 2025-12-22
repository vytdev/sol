/* ascii stuff helper. */

#ifndef SOL_CHAR_H_
#define SOL_CHAR_H_

#define TABSTOP 8

#define SP   ' '
#define TAB  '\t'
#define LF   '\n'
#define CR   '\r'

#define is_wspace(a)   ((a) == SP || (a) == TAB || (a) == LF || (a) == CR)
#define is_digit(a)    ((a) >= '0' && (a) <= '9')
#define is_loalpha(a)  ((a) >= 'a' && (a) <= 'z')
#define is_upalpha(a)  ((a) >= 'A' && (a) <= 'Z')
#define is_alpha(a)    (is_loalpha((a)) || is_upalpha((a)))
#define is_alphanum(a) (is_alpha((a)) || is_digit((a)))
#define is_ident(a)    (is_alpha((a)) || (a) == '_' || (a) == '$')

#endif /* SOL_CHAR_H_ */
