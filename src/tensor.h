#ifndef TENSOR_H
#define TENSOR_H

#include "platform.h"
#include "allocator.h"

namespace welkin
{
    class WELKIN_EXPORT Tensor
    {
    public:
        Tensor();
        // vec
        Tensor(int n, int w, size_t elemsize = 4u, Allocator *allocator = 0);

        Tensor(int n, int w, int h, size_t elemsize = 4u, Allocator *allocator = 0);

        Tensor(int n, int w, int h, int c, size_t elemsize = 4u, Allocator *allocator = 0);
        // copy
        Tensor(const Tensor &t);

        Tensor(int n, int w, void *data, size_t elemsize = 4u, Allocator *alocator = 0);

        Tensor(int n, int w, int h, void *data, size_t elemsize = 4u, Allocator *allocator = 0);

        Tensor(int n, int w, int h, int c, void *data, size_t elemsize = 4u, Allocator *allocator = 0);
        // release
        ~Tensor();

    public:
        // copy assign
        Tensor &operator=(const Tensor &t);
        // deep copy
        Tensor clone(Allocator *allocator = 0) const;

        void clone_from(const Tensor &t, Allocator *allocator = 0);

        Tensor reshape(int w, Allocator *allocator = 0) const;

        Tensor reshape(int w, int h, Allocator *allocator = 0) const;

        Tensor reshape(int w, int h, int c, Allocator *allocator = 0) const;

        void create(int n, int w, size_t elemsize = 4u, Allocator *allocator = 0);

        void create(int n, int w, int h, size_t elemsize = 4u, Allocator *allocator = 0);

        void create(int n, int w, int h, int c, size_t elemsize = 4u, Allocator *allocator = 0);

        // refcount++
        void addref();
        // refcount--
        void release();

        bool empty() const;

        size_t total() const { return m_cstep * m_c * m_num; }

        // get shape
        Tensor shape() const;

        // data reference
        const Tensor channel(int n, int c) const;

        Tensor channel(int n, int c);

        const float *row(int y) const;

        float *row(int y);

        template <typename T>
        const T *row(int y) const;

        template <typename T>
        T *row(int y);

        // access raw data
        template <typename T>
        operator const T *() const;

        template <typename T>
        operator T *();

        // fill data
        void fill(float v);

        template <typename T>
        void fill(T val);

        // convenient access float data
        float &operator[](size_t i);

        const float &operator[](size_t i) const;

    public:
        // pointer to the data
        void *m_data;

        // reference counter
        int *m_refcount;

        // float32/int32 = 4
        // float16 = 2
        // int8/uint8 = 1
        // empty = 0
        size_t m_elemsize;

        // the allocator
        Allocator *m_allocator;

        // the dimension rank
        int m_dims;

        int m_w;
        int m_h;
        int m_c;
        int m_num;

        size_t m_cstep;
    };
} // namespace welkin

#endif