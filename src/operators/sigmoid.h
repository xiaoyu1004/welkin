#ifndef _INPLACESOP_H_
#define _INPLACESOP_H_

#include <vector>

#include "operator.h"
#include "tensor.h"

namespace welkin
{
    class Sigmoid : public Operator
    {
    public:
        virtual void forward(const std::vector<Tensor *> &inputs, std::vector<Tensor *> &outputs);
        virtual void backward(std::vector<Tensor *> &inputs, const std::vector<Tensor *> &outputs);
    };
} // namespace welkin

#endif