#include "byte.h"

unsigned long int byte_chr(__s,n,c)
const void *__s;
unsigned long int n;
int c;
{
  char ch;
  char *t;
  const char *s = __s;

  ch = c;
  t = s;
  for (;;) {
    if (!n) break; if (*t == ch) break; ++t; --n;
    if (!n) break; if (*t == ch) break; ++t; --n;
    if (!n) break; if (*t == ch) break; ++t; --n;
    if (!n) break; if (*t == ch) break; ++t; --n;
  }
  return t - s;
}
