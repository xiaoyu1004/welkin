#include <cmath>

#include "sigmoid.h"
#include "exception.h"

namespace welkin
{
    void Sigmoid::forward(const std::vector<Tensor *> &inputs, std::vector<Tensor *> &outputs)
    {
        // KIERA_CHECK(inputs.size() == outputs.size(), "input size must be equal with output");
        // KIERA_CHECK(inputs.size() > 0, "input size must be greater than zero");
        // for (size_t i = 0; i < inputs.size(); ++i)
        // {
        //     const Tensor *input = inputs[i];
        //     Tensor *output = outputs[i];
        //     KIERA_CHECK(input->shape() == output->shape(), "input shape must be equal with output shape");
        //     const DataType &in_data = input->data();
        //     DataType &out_data = output->data();
        //     for (size_t k = 0; k < in_data.size(); ++k)
        //     {
        //         out_data[k] = 1.f / (1.f + std::exp(-in_data[k]));
        //     }
        // }
    }

    void Sigmoid::backward(std::vector<Tensor *> &inputs, const std::vector<Tensor *> &outputs)
    {
        // KIERA_CHECK(inputs.size() == outputs.size(), "input size must be equal with output");
        // KIERA_CHECK(inputs.size() > 0, "input size must be greater than zero");
        // for (size_t i = 0; i < inputs.size(); ++i)
        // {
        //     Tensor *input = inputs[i];
        //     const Tensor *output = outputs[i];
        //     KIERA_CHECK(input->shape() == output->shape(), "input shape must be equal with output shape");
        //     DataType &in_data = input->data();
        //     const DataType &out_data = output->data();
        //     for (size_t k = 0; k < in_data.size(); ++k)
        //     {
        //         in_data[k] = out_data[k] * (1 - out_data[k]);
        //     }
        // }
    }
} // namespace welkin