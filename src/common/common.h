#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <cstdarg>
#include <cstdio>
#include "platform.h"

namespace welkin
{
#define WELKIN_INFO(format, ...) printf(format, ##__VA_ARGS__)

#define WELKIN_ASSERT(condition, fmt, ...)   \
    {                                        \
        int res = (condition);               \
        if (!res)                            \
        {                                    \
            WELKIN_INFO(fmt, ##__VA_ARGS__); \
        }                                    \
    }
} // namespace welkin

#endif