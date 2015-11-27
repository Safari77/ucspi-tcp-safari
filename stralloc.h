#ifndef STRALLOC_H
#define STRALLOC_H

#include "alloc.h"
#include "gen_alloc.h"
#include "byte.h"
#include "str.h"
#include "stralloc.h"

GEN_ALLOC_typedef(stralloc,char,s,len,a)

extern int stralloc_ready(stralloc *,unsigned int);
extern int stralloc_readyplus(stralloc *,unsigned int);
extern int stralloc_append(stralloc *,char *); /* beware: this takes a pointer to 1 char */
extern int stralloc_starts(stralloc *,char *);

#define stralloc_0(sa) stralloc_append(sa,"")

extern int stralloc_catulong0(stralloc *,unsigned long,unsigned int);
extern int stralloc_catlong0(stralloc *,long,unsigned int);

static inline int stralloc_copyb(stralloc *sa,char *s,unsigned int n)
{
  if (!stralloc_ready(sa,n + 1)) return 0;
  byte_copy(sa->s,n,s);
  sa->len = n;
  sa->s[n] = 'Z'; /* ``offensive programming'' */
  return 1;
}

static inline int stralloc_copys(stralloc *sa,char *s)
{
  return stralloc_copyb(sa,s,str_len(s));
}

static inline int stralloc_copy(stralloc *sato,stralloc *safrom)
{
  return stralloc_copyb(sato,safrom->s,safrom->len);
}

static inline int stralloc_catb(stralloc *sa,char *s,unsigned int n)
{
  if (!sa->s) return stralloc_copyb(sa,s,n);
  if (!stralloc_readyplus(sa,n + 1)) return 0;
  byte_copy(sa->s + sa->len,n,s);
  sa->len += n;
  sa->s[sa->len] = 'Z'; /* ``offensive programming'' */
  return 1;
}

static inline int stralloc_cats(stralloc *sa,char *s)
{
  return stralloc_catb(sa,s,str_len(s));
}

static inline int stralloc_cat(stralloc *sato,stralloc *safrom)
{
  return stralloc_catb(sato,safrom->s,safrom->len);
}

#define stralloc_catlong(sa,l) (stralloc_catlong0((sa),(l),0))
#define stralloc_catuint0(sa,i,n) (stralloc_catulong0((sa),(i),(n)))
#define stralloc_catint0(sa,i,n) (stralloc_catlong0((sa),(i),(n)))
#define stralloc_catint(sa,i) (stralloc_catlong0((sa),(i),0))

#endif
