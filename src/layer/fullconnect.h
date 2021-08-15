#ifndef _DENSEOP_H_
#define _DENSEOP_H_

#include "layer.h"
#include "tensor.h"

namespace welkin
{
    class FullConnect : public Layer
    {
    public:
        FullConnect(){};
        ~FullConnect(){};

    public:
        virtual void setup(int in_dim, int out_dim, bool with_bias) override;
        virtual void forward(const std::vector<Tensor *> &inputs, std::vector<Tensor *> &outputs) override;
        virtual void backward(std::vector<Tensor *> &inputs, const std::vector<Tensor *> &outputs) override;

    private:
        // inner paramters
        Tensor *weights;
        Tensor *bias;
    };
} // namespace welkin
#endif