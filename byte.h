#ifndef BYTE_H
#define BYTE_H

#include <string.h>

extern unsigned long int byte_chr(const void*, unsigned long int, int);
extern unsigned long int byte_rchr(const void*, unsigned long int, int);
extern void byte_copyr(void*, unsigned long int, const void*);

#define byte_copy(t,n,f) memcpy((t),(f),(n))
#define byte_zero(t,n) memset(t, 0, n)
#define byte_diff(t,n,f) memcmp((t),(f),(n))
#define byte_equal(s,n,t) (!byte_diff((s),(n),(t)))

#endif

