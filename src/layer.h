#ifndef LAYER_H
#define LAYER_H

#include "tensor.h"

namespace welkin
{
    class Layer
    {
    public:
        Layer() {}
        virtual ~Layer() {}
        // todo add weights, grad, etc

    public:
        void setup(int in_dim, int out_dim, bool with_bias);
        virtual void forward(const std::vector<Tensor *> &inputs, std::vector<Tensor *> &outputs) = 0;
        virtual void backward(std::vector<Tensor *> &inputs, const std::vector<Tensor *> &outputs) = 0;
    };
} // namespace welkin
#endif