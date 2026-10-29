#ifndef ULTRA_LIBC_XSTDIO_H
#define ULTRA_LIBC_XSTDIO_H

typedef char *va_list;
typedef unsigned int size_t;

#define _FP 1
#define _VA_FP_SAVE_AREA 0x10
#define _VA_ALIGN(p, a) (((unsigned int)(((char *)p) + ((a) > 4 ? (a) : 4) - 1)) & -((a) > 4 ? (a) : 4))

#define __va_stack_arg(list, mode)                                                             \
    (((list) = (char *)_VA_ALIGN(list, __builtin_alignof(mode)) + _VA_ALIGN(sizeof(mode), 4)), \
     (((char *)list) - (_VA_ALIGN(sizeof(mode), 4) - sizeof(mode))))

#define __va_double_arg(list, mode)                                                                                \
    ((((long)list & 0x1)                                                                                           \
          ? (list = (char *)((long)list + 7), (char *)((long)list - 6 - _VA_FP_SAVE_AREA))                         \
          : (((long)list & 0x2) ? (list = (char *)((long)list + 10), (char *)((long)list - 24 - _VA_FP_SAVE_AREA)) \
                                : __va_stack_arg(list, mode))))

#define va_arg(list, mode)                                                                                          \
    ((mode *)((                                                                                                     \
        (__builtin_classof(mode) == _FP && __builtin_alignof(mode) == sizeof(double)) ? __va_double_arg(list, mode) \
                                                                                      : __va_stack_arg(list, mode)  \
    )))[-1]

#ifdef __sgi
typedef double ldouble;
#else
typedef long double ldouble;
#endif

typedef struct {
    union {
        long long ll;
        ldouble ld;
    } v;
    unsigned char *s;
    int n0;
    int nz0;
    int n1;
    int nz1;
    int n2;
    int nz2;
    int prec;
    int width;
    size_t nchar;
    unsigned int flags;
    char qual;
} _Pft;

#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16

int _Printf(void *pfn(void *, const char *, size_t), void *arg, const char *fmt, va_list ap);
void _Litob(_Pft *px, char code);
void _Ldtob(_Pft *px, char code);

#endif
