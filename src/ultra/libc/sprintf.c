typedef char *va_list;
#define va_start(ap, param) (ap = (va_list) & param + sizeof(param))
#define va_end(ap)

typedef unsigned int size_t;

void *memcpy(void *s1, const void *s2, size_t n);

int _Printf(void *(*pfn)(void *, const char *, size_t), void *arg, const char *fmt, va_list ap);

static void *proutSprintf(void *s, const char *buf, size_t n) {
    return (char *)memcpy(s, buf, n) + n;
}

int sprintf(char *s, const char *fmt, ...) {
    int ans;
    va_list ap;

    va_start(ap, fmt);
    ans = _Printf(proutSprintf, s, fmt, ap);
    if (ans >= 0) {
        s[ans] = 0;
    }
    return ans;
}
