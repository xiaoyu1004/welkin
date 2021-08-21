#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace welkin
{
#define WELKIN_INFO(format, ...)       \
    {                                  \
        printf(format, ##__VA_ARGS__); \
        printf("\n");                  \
    }

#define WELKIN_CHECK(condition, fmt, ...)    \
    {                                        \
        int res = (condition);               \
        if (!res)                            \
        {                                    \
            WELKIN_INFO(fmt, ##__VA_ARGS__); \
        }                                    \
    }
} // namespace welkin

#endif