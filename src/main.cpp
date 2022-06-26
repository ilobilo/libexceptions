#include <stdint.h>
#include <stddef.h>

#define WEAK __attribute__((weak))
#define WEAK_NORETURN __attribute__((weak, noreturn))

extern "C"
{
    void *malloc(size_t size);
    void *calloc(size_t n, size_t m);
    void *realloc(void *ptr, size_t new_size);
    void free(void *ptr);

    void *stderr WEAK = nullptr;

    WEAK_NORETURN void abort()
    {
        while (true) asm volatile ("nop");
        __builtin_unreachable();
    }

    WEAK void *memcpy(void *dest, const void *src, size_t len)
    {
        uint8_t *pdest = static_cast<uint8_t*>(dest);
        const uint8_t *psrc = static_cast<const uint8_t*>(src);

        for (size_t i = 0; i < len; i++)
        {
            pdest[i] = psrc[i];
        }

        return dest;
    }

    WEAK void *memset(void *dest, int ch, size_t len)
    {
        uint8_t *p = static_cast<uint8_t*>(dest);

        for (size_t i = 0; i < len; i++)
        {
            p[i] = static_cast<uint8_t>(ch);
        }

        return dest;
    }

    WEAK void *memmove(void *dest, const void *src, size_t len)
    {
        uint8_t *pdest = static_cast<uint8_t*>(dest);
        const uint8_t *psrc = static_cast<const uint8_t*>(src);

        if (src > dest)
        {
            for (size_t i = 0; i < len; i++) pdest[i] = psrc[i];
        }
        else if (src < dest)
        {
            for (size_t i = len; i > 0; i--) pdest[i-1] = psrc[i-1];
        }

        return dest;
    }

    WEAK void *memchr(const void *ptr, int ch, size_t len)
    {
        const uint8_t *src = static_cast<const uint8_t*>(ptr);

        while (len-- > 0)
        {
            if (*src == ch) return const_cast<uint8_t*>(src);
            src++;
        }

        return nullptr;
    }

    WEAK size_t strlen(const char *str)
    {
        if (str == nullptr) return 0;
        size_t length = 0;
        while (str[length]) length++;
        return length;
    }

    WEAK int strcmp(const char *str1, const char *str2)
    {
        while (*str1 && *str2 && *str1 == *str2)
        {
            str1++;
            str2++;
        }
        return *str1 - *str2;
    }

    WEAK int strncmp(const char *str1, const char *str2, size_t len)
    {
        while (*str1 && *str2 && *str1 == *str2 && len--)
        {
            str1++;
            str2++;
        }
        if (len == 0) return 0;
        return *str1 - *str2;
    }

#define X(x) (((x) / 256 | (x) * 256) % 65536)
    static const unsigned short table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200),
        X(0x200), X(0x320), X(0x220), X(0x220), X(0x220), X(0x220), X(0x200), X(0x200),
        X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200),
        X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200), X(0x200),
        X(0x160), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0),
        X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0),
        X(0x8D8), X(0x8D8), X(0x8D8), X(0x8D8), X(0x8D8), X(0x8D8), X(0x8D8), X(0x8D8),
        X(0x8D8), X(0x8D8), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0),
        X(0x4C0), X(0x8D5), X(0x8D5), X(0x8D5), X(0x8D5), X(0x8D5), X(0x8D5), X(0x8C5),
        X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5),
        X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5), X(0x8C5),
        X(0x8C5), X(0x8C5), X(0x8C5), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0),
        X(0x4C0), X(0x8D6), X(0x8D6), X(0x8D6), X(0x8D6), X(0x8D6), X(0x8D6), X(0x8C6),
        X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6),
        X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6), X(0x8C6),
        X(0x8C6), X(0x8C6), X(0x8C6), X(0x4C0), X(0x4C0), X(0x4C0), X(0x4C0), X(0x200),
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
#undef X

    static const unsigned short *const ptable = table + 128;
    const unsigned short **__ctype_b_loc(void)
    {
        return const_cast<const unsigned short**>(&ptable);
    }

    int posix_memalign(void **ptr, size_t align, size_t len)
    {
        if (align < sizeof(void*)) return 22;
        *ptr = malloc((len + align - 1) & ~(align - 1));
        return 0;
    }

    uintptr_t __stack_chk_guard WEAK = 0x595E9FBD94FDA766;
    WEAK_NORETURN void __stack_chk_fail()
    {
        abort();
    }

    WEAK int __snprintf_chk(char *str, size_t maxlen, int flag, size_t len, const char *format, ...)
    {
        if (len < maxlen) return -1;
        return 0;
    }

    WEAK int __fprintf_chk(void *fp, int flag, const char *format, ...)
    {
        return 0;
    }

    WEAK int fflush(void *fp)
    {
        return 0;
    }

    WEAK char *getenv(const char *name)
    {
        return const_cast<char*>("");
    }

    WEAK_NORETURN void __assert_func(const char *file, int line, const char *func, const char *message)
    {
        abort();
    }

    WEAK_NORETURN void __assert_fail(const char *message, const char *file, int line, const char *func)
    {
        abort();
    }
} // extern "C"