#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "solc.h"
#include "sol.h"


#define CODESZ 8192

int main (int argc, char **argv)
{
  if (argc < 2) {
    printf("usage: %s FILE\n", argv[0]);
    return 1;
  }

  size_t len = 0;
  char *buf = readfile(argv[1], &len);
  if (!buf) {
    fprintf(stderr, "%s: Failed to open file: %s\n", argv[0], argv[1]);
    return 1;
  }

  char *code = (char*)malloc(CODESZ);
  if (!code) {
    free(buf);
    fprintf(stderr, "%s: Could not allocate code buffer\n", argv[0]);
    return 1;
  }

  compile_err errs[20];

  // compile
  solc C;
  solc_init(&C, buf, len, code, CODESZ);
  solc_seterrbuf(&C, errs, 20);
  int err = solc_compile(&C);
  int printable_err = err > 20 ? 20 : err;   // prevent accessing beyond errs[]
  for (int i = 0; i < printable_err; i++)
    msgtok(&C, &C.err[i].token, "%s", C.err[i].msg);
  printf("compile err cnt: %d\n", err);
  if (err != CSUCC) {
    free(code);
    free(buf);
    return err;
  }

  // run
  sol T;
  sol_init(&T, code, C.cpos);
  int stat = sol_exec(&T);
  printf("exec stat: %d\n", stat);

  // print result
  tvalue_t tv;
  sol_pop(&T, &tv);
  printf("interp result: %ld\n", tv.v.intv);

  free(code);
  free(buf);
  return 0;
}
