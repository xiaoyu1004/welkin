#ifndef OPERATOR_H
#define OPERATOR_H

#include "tensor.h"

namespace welkin
{
    template <class Context>
    class Operator
    {
    public:
        Operator() {}
        virtual ~Operator() {}

    public:
        bool Run(int stream_id = 0) final
        {
            m_context->SwitchToDevice(stream_id);

        }

        virtual bool RunOnDevice() = 0;

        const Context *getContext() const
        {
            return &m_context;
        }

        Context *getContext()
        {
            return &m_context;
        }

    private:
        Context m_context;
    };
} // namespace welkin

#endif