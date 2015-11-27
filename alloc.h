#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>
#include "error.h"
#include "helper.h"

static inline /*@null@*//*@out@*/void * __must_check alloc(n)
size_t n;
{
  void *x;
  if (!n) n = sizeof(unsigned long);
  x = malloc(n);
  if (!x) errno = error_nomem;
  return x;
}

static inline void alloc_free(x)
void *x;
{
  free(x);
}

extern int __must_check alloc_re(void **, size_t, size_t);

#endif
