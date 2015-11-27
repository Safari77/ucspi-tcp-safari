#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdint.h>

#include "uint16.h"
#include "uint32.h"
#include "uint64.h"

#define ALIGN(x,a)		__ALIGN_MASK(x,(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)	(((x)+(mask))&~(mask))
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#define bos0(dest) __builtin_object_size(dest, 0)
/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define djb_roundup(x, y) ((((x) + ((y) - 1)) / (y)) * (y))
#define noinline __attribute__((noinline))
#undef inline
#undef __inline__
#undef __inline
#undef __always_inline
#define __always_inline         inline __attribute__((always_inline))

#undef __must_check

#ifndef __INTEL_COMPILER
#if (__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#define __must_check __attribute__((warn_unused_result))
#else
#define __must_check
#endif
#else
#define __must_check
#endif

/*
 * min()/max() macros that also do
 * strict type-checking.. See the
 * "unnecessary" pointer comparison.
 */
#define min(x,y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x < _y ? _x : _y; })

#define max(x,y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x > _y ? _x : _y; })

#define prefetch(x) __builtin_prefetch(x)

struct __una_u16 { uint16 x __attribute__((packed)); };
struct __una_u32 { uint32 x __attribute__((packed)); };
struct __una_u64 { uint64 x __attribute__((packed)); };

static inline uint16 __get_unaligned_cpu16(const void *p)
{
        const struct __una_u16 *ptr = (const struct __una_u16 *)p;
        return ptr->x;
}

static inline uint32 __get_unaligned_cpu32(const void *p)
{
        const struct __una_u32 *ptr = (const struct __una_u32 *)p;
        return ptr->x;
}

static inline uint64 __get_unaligned_cpu64(const void *p)
{
        const struct __una_u64 *ptr = (const struct __una_u64 *)p;
        return ptr->x;
}

static inline void __put_unaligned_cpu16(uint16 val, void *p)
{
        struct __una_u16 *ptr = (struct __una_u16 *)p;
        ptr->x = val;
}

static inline void __put_unaligned_cpu32(uint32 val, void *p)
{
        struct __una_u32 *ptr = (struct __una_u32 *)p;
        ptr->x = val;
}

static inline void __put_unaligned_cpu64(uint64 val, void *p)
{
        struct __una_u64 *ptr = (struct __una_u64 *)p;
        ptr->x = val;
}

/*
 * ..and if you can't take the strict
 * types, you can specify one yourself.
 *
 * Or not use min/max at all, of course.
 */
#define min_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define max_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

//#define offsetof(___TYPE, ___MEMBER) __builtin_offsetof(___TYPE, ___MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - offsetof(type,member) );})

/*
 * Check at compile time that something is of a particular type.
 * Always evaluates to 1 so you may use it easily in comparisons.
 */
#define typecheck(type,x) \
({	type __dummy; \
	typeof(x) __dummy2; \
	(void)(&__dummy == &__dummy2); \
	1; \
})

/*
 * Check at compile time that 'function' is a certain type, or is a pointer
 * to that type (needs to use typedef for the function type.)
 */
#define typecheck_fn(type,function) \
({	typeof(type) __tmp = function; \
	(void)__tmp; \
})

#endif
