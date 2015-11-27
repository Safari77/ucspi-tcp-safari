#include "byte.h"

void byte_copyr(__to,n,__from)
void *__to;
unsigned long int n;
const void *__from;
{
  char *to = __to;
  const char *from = __from;
  to += n;
  from += n;
  for (;;) {
    if (!n) return; *--to = *--from; --n;
    if (!n) return; *--to = *--from; --n;
    if (!n) return; *--to = *--from; --n;
    if (!n) return; *--to = *--from; --n;
  }
}
