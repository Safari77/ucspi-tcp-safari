#include "fmt.h"
#include "uint64.h"

unsigned int fmt_ullint(s,u) register char *s; register uint64 u;
{
  register uint64 l;

  l = u;
  return fmt_ullong(s,l);
}
