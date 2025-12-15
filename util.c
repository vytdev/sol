#include "util.h"
#include <stdlib.h>
#include <stdio.h>

char *readfile(char *path, size_t *len)
{
  FILE *fp = fopen(path, "rb");
  if (!fp)
    return NULL;

  fseek(fp, 0, SEEK_END);
  size_t sz = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (sz == 0) {
    fclose(fp);
    return NULL;
  }

  char *buf = (char*)malloc(sz+1);
  if (!buf) {
    fclose(fp);
    return NULL;
  }
  buf[sz] = '\0';

  size_t in = fread(buf, 1, sz, fp);
  if (in != sz) {
    fclose(fp);
    free(buf);
    return NULL;
  }

  fclose(fp);
  if (len)
    *len = sz;
  return buf;
}
