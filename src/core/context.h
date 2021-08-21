#ifndef CONTEXT_H
#define CONTEXT_H

#include "platform.h"
#include "device.h"

namespace welkin
{
    class WELKIN_EXPORT Context
    {
    public:
        Context();
        virtual ~Context() noexcept {}

    public:
        void SwitchToDevice(int d);
    };
}

#endif