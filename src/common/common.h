#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <cstdarg>
#include <cstdio>
#include "platform.h"

namespace welkin
{
#define WELKIN_CHECK(condition, fmt, ...) welkin_check(condition, fmt, ##__VA_ARGS__)

    static void welkin_check(const bool condition, const char *fmt, ...)
    {
        if (!condition)
        {
            va_list ap;
            va_start(ap, fmt);
            fprintf(stderr, fmt, ap);
            printf("\n");
            va_end(ap);
        }
    }
} // namespace welkin

#endif