#include "byte.h"

unsigned long int byte_rchr(__s,n,c)
const void *__s;
unsigned long int n;
int c;
{
  char ch;
  char *t;
  char *u;
  const char *s = __s;

  ch = c;
  t = s;
  u = 0;
  for (;;) {
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
    if (!n) break; if (*t == ch) u = t; ++t; --n;
  }
  if (!u) u = t;
  return u - s;
}
