#include <stdlib.h>
#include "alloc.h"
#include "byte.h"

int alloc_re(x,m,n)
void **x;
size_t m;
size_t n;
{
  void *y;

  y = malloc(n);
  if (!y) return 0;
  byte_copy(y,m,*x);
  free(*x);
  *x = y;
  return 1;
}

