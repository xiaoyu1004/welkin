#include <cstdlib>

#include "tensor.h"
#include "allocator.h"

namespace welkin
{
    Tensor::Tensor()
        : m_num(0), m_w(0), m_h(0), m_c(0), m_dims(0), m_allocator(nullptr), m_cstep(0), m_refcount(nullptr), m_data(nullptr), m_elemsize(0)
    {
    }

    Tensor::Tensor(int n, int w, size_t elemsize, Allocator *allocator)
        : m_num(0), m_w(0), m_h(0), m_c(0), m_dims(0), m_allocator(nullptr), m_cstep(0), m_refcount(nullptr), m_data(nullptr), m_elemsize(elemsize)
    {
        create(n, w, elemsize, allocator);
    }

    Tensor::Tensor(int n, int w, int h, size_t elemsize, Allocator *allocator)
        : m_num(0), m_w(0), m_h(0), m_c(0), m_dims(0), m_allocator(nullptr), m_cstep(0), m_refcount(nullptr), m_data(nullptr), m_elemsize(elemsize)
    {
        create(n, w, h, elemsize, allocator);
    }

    Tensor::Tensor(int n, int w, int h, int c, size_t elemsize, Allocator *allocator)
        : m_num(0), m_w(0), m_h(0), m_c(0), m_dims(0), m_allocator(nullptr), m_cstep(0), m_refcount(nullptr), m_data(nullptr), m_elemsize(elemsize)
    {
        create(n, w, h, c, elemsize, allocator);
    }

    Tensor::Tensor(const Tensor &t)
        : m_num(t.m_num), m_w(t.m_w), m_h(t.m_h), m_c(t.m_c), m_dims(t.m_dims), m_allocator(t.m_allocator), m_cstep(t.m_cstep), m_refcount(t.m_refcount), m_data(t.m_data), m_elemsize(t.m_elemsize)
    {
        addref();
    }

    Tensor::Tensor(int n, int w, void *data, size_t elemsize, Allocator *alocator)
        : m_num(n), m_w(w), m_h(1), m_c(1), m_dims(1), m_allocator(alocator), m_cstep(w), m_refcount(nullptr), m_data(data), m_elemsize(elemsize)
    {
    }

    Tensor::Tensor(int n, int w, int h, void *data, size_t elemsize, Allocator *allocator)
        : m_num(n), m_w(w), m_h(h), m_c(1), m_dims(2), m_allocator(allocator), m_refcount(nullptr), m_data(data), m_elemsize(elemsize)
    {
        m_cstep = (size_t)(w * h);
    }

    Tensor::Tensor(int n, int w, int h, int c, void *data, size_t elemsize, Allocator *allocator)
        : m_num(n), m_w(w), m_h(h), m_c(c), m_dims(3), m_allocator(allocator), m_refcount(nullptr), m_data(data), m_elemsize(elemsize)
    {
        m_cstep = alignSize((size_t)(w * h) * elemsize, MALLOC_ALIGN) / elemsize;
    }

    Tensor::~Tensor()
    {
        release();
    }

    Tensor &Tensor::operator=(const Tensor &t)
    {
        // check self assign
        if (this == &t)
        {
            return *this;
        }
        // alter refcount
        if (t.m_refcount)
        {
            WELKIN_XADD(t.m_refcount, 1);
        }
        // assign
        m_data = t.m_data;
        m_num = t.m_num;
        m_w = t.m_w;
        m_h = t.m_h;
        m_c = t.m_c;
        m_dims = t.m_dims;
        m_cstep = t.m_cstep;

        m_elemsize = t.m_elemsize;
        m_allocator = t.m_allocator;
        m_refcount = t.m_refcount;

        return *this;
    }

    Tensor Tensor::clone(Allocator *allocator) const
    {
        Tensor t;
        if (empty())
        {
            return t;
        }
        if (m_dims == 1)
        {
            t.create(m_num, m_w, m_elemsize, m_allocator);
        }
        else if (m_dims == 2)
        {
            t.create(m_num, m_w, m_h, m_elemsize, m_allocator);
        }
        else if (m_dims == 3)
        {
            t.create(m_num, m_w, m_h, m_c, m_elemsize, m_allocator);
        }
        // copy data
        memcpy(t.m_data, m_data, total() * m_elemsize);
        return t;
    }

    void Tensor::clone_from(const Tensor &t, Allocator *allocator)
    {
        *this = t.clone(allocator);
    }

    Tensor Tensor::reshape(int w, Allocator *allocator) const
    {
        Tensor t;
        // check
        if (m_w * m_h * m_c != w)
        {
            return t;
        }
        // malloc new memory, when between of channels exist gap
        if (m_dims == 3 && m_cstep != (size_t)(m_w * m_h))
        {
            t.create(m_num, w, m_elemsize, allocator);

            for (int i = 0, size = m_num * m_c; i < size; ++i)
            {
                unsigned char *ptr = (unsigned char *)m_data + i * m_cstep * m_elemsize;
                unsigned char *tptr = (unsigned char *)t.m_data + i * m_w * m_h * m_elemsize;
                memcpy(tptr, ptr, m_w * m_h * m_elemsize);
            }
            return t;
        }

        // don't malloc new memory
        t = *this;

        t.m_w = w;
        t.m_h = 1;
        t.m_c = 1;

        t.m_dims = 1;
        t.m_cstep = w;
        return t;
    }

    Tensor Tensor::reshape(int w, int h, Allocator *allocator) const
    {
        Tensor t;
        if (m_w * m_h * m_c != w * h)
        {
            return t;
        }
        if (m_dims == 3 && m_cstep != (size_t)(m_w * m_h))
        {
            t.create(m_num, w, h, m_elemsize, allocator);

            for (int i = 0, size = m_num * m_c; i < size; ++i)
            {
                unsigned char *ptr = (unsigned char *)m_data + i * m_cstep * m_elemsize;
                unsigned char *tptr = (unsigned char *)t.m_data + i * m_w * m_h * m_elemsize;
                memcpy(tptr, ptr, m_w * m_h * m_elemsize);
            }
            return t;
        }

        t = *this;
        t.m_dims = 2;
        t.m_w = w;
        t.m_h = h;
        t.m_c = 1;
        t.m_cstep = (size_t)(w * h);

        return t;
    }

    Tensor Tensor::reshape(int w, int h, int c, Allocator *allocator) const
    {
        Tensor t;
        if (m_w * m_h * m_c != w * h * c)
        {
            return t;
        }
        // need alloc new memory
        if (m_dims < 3 && (w * h) % MALLOC_ALIGN != 0)
        {
            t.create(m_num, w, h, c, m_elemsize, allocator);

            for (int i = 0, size = m_num * c; i < size; ++i)
            {
                unsigned char *ptr = (unsigned char *)m_data + i * w * h * m_elemsize;
                unsigned char *tptr = (unsigned char *)t.m_data + i * t.m_cstep * m_elemsize;
                memcpy(tptr, ptr, w * h * m_elemsize);
            }
            return t;
        }
        else if (m_c != c)
        {
            Tensor tmp = reshape(w * h * c, allocator);
            return tmp.reshape(w, h, c, allocator);
        }
        // don't need alloc new momery
        t.m_w = w;
        t.m_h = h;
        t.m_c = c;
        t.m_dims = 3;
        t.m_cstep = alignSize((size_t)(w * h), MALLOC_ALIGN);

        return t;
    }

    void Tensor::create(int n, int w, size_t elemsize, Allocator *allocator)
    {
        if (m_dims == 1 && m_num == n && m_w == w && m_elemsize == elemsize && m_allocator == allocator)
        {
            return;
        }
        release();

        m_elemsize = elemsize;
        m_allocator = allocator;

        m_dims = 1;
        m_num = n;
        m_w = w;
        m_h = 1;
        m_c = 1;
        m_cstep = w;

        size_t totalsize = alignSize(total() * elemsize, 4);
        if (allocator)
        {
            m_data = allocator->fastMalloc(totalsize + sizeof(int));
        }
        else
        {
            m_data = fastMalloc(totalsize + sizeof(int));
        }
        m_refcount = (int *)((unsigned char *)m_data + totalsize);
        *m_refcount = 1;
    }

    void Tensor::create(int n, int w, int h, size_t elemsize, Allocator *allocator)
    {
        if (m_dims == 2 && m_num == n && m_w == w && m_h == h && m_elemsize == elemsize && m_allocator == allocator)
        {
            return;
        }
        release();

        m_elemsize = elemsize;
        m_allocator = allocator;

        m_dims = 2;
        m_num = n;
        m_w = w;
        m_h = h;
        m_c = 1;
        m_cstep = (size_t)(w * h);

        size_t totalsize = alignSize(total() * elemsize, 4);
        if (allocator)
        {
            m_data = allocator->fastMalloc(totalsize);
        }
        else
        {
            m_data = fastMalloc(totalsize);
        }
        m_refcount = (int *)((unsigned char *)m_data + totalsize);
        *m_refcount = 1;
    }

    void Tensor::create(int n, int w, int h, int c, size_t elemsize, Allocator *allocator)
    {
        if (m_dims == 3 && m_num == n && m_w == w && m_h == h && m_c == c && m_elemsize == elemsize && m_allocator == allocator)
        {
            return;
        }
        release();

        m_elemsize = elemsize;
        m_allocator = allocator;

        m_dims = 3;
        m_num = n;
        m_w = w;
        m_h = h;
        m_c = c;

        m_cstep = alignSize((size_t)(w * h * elemsize), 16) / elemsize;
        size_t totalsize = alignSize(total() * elemsize, 4);
        if (allocator)
        {
            m_data = allocator->fastMalloc(totalsize + sizeof(int));
        }
        else
        {
            m_data = fastMalloc(totalsize + sizeof(int));
        }
        m_refcount = (int *)((unsigned char *)m_data + totalsize);
        *m_refcount = 1;
    }

    void Tensor::addref()
    {
        if (m_refcount)
        {
            WELKIN_XADD(m_refcount, 1);
        }
    }

    void Tensor::release()
    {
        if (m_refcount && WELKIN_XADD(m_refcount, -1) == 1)
        {
            if (m_allocator)
            {
                m_allocator->fastFree(m_data);
            }
            else
            {
                fastFree(m_data);
            }
        }

        m_dims = 0;
        m_num = 0;
        m_w = 0;
        m_h = 0;
        m_c = 0;

        m_elemsize = 0;
        m_allocator = nullptr;
        m_refcount = nullptr;
        m_data = nullptr;
        m_cstep = 0;
    }

    bool Tensor::empty() const
    {
        return m_data == nullptr || total() == 0;
    }

    Tensor Tensor::shape() const
    {
        if (m_dims)
        {
            return Tensor(m_num, m_w, m_h, m_c, nullptr);
        }
        return Tensor();
    }

    const Tensor Tensor::channel(int n, int c) const
    {
        return Tensor(n, m_w, m_h, (unsigned char *)m_data + (n * m_c + c) * m_cstep * m_elemsize, m_elemsize, m_allocator);
    }

    Tensor Tensor::channel(int n, int c)
    {
        return Tensor(n, m_w, m_h, (unsigned char *)m_data + (n * m_c + c) * m_cstep * m_elemsize, m_elemsize, m_allocator);
    }

    const float *Tensor::row(int y) const
    {
        return (float *)((unsigned char *)m_data + m_w * y * m_elemsize);
    }

    float *Tensor::row(int y)
    {
        return (float *)((unsigned char *)m_data + m_w * y * m_elemsize);
    }

    template <typename T>
    const T *Tensor::row(int y) const
    {
        return (T *)((unsigned char *)m_data + m_w * y * m_elemsize);
    }

    template <typename T>
    T *Tensor::row(int y)
    {
        return (T *)((unsigned char *)m_data + m_w * y * m_elemsize);
    }

    template <typename T>
    Tensor::operator const T *() const
    {
        return (const T *)m_data;
    }

    template <typename T>
    Tensor::operator T *()
    {
        return (T *)m_data;
    }

    void Tensor::fill(float v)
    {
        float *ptr = (float *)m_data;
        for (size_t i = 0, size = total(); i < size; ++i)
        {
            ptr[i] = v;
        }
    }

    template <typename T>
    void Tensor::fill(T v)
    {
        T *ptr = (T *)m_data;
        for (int i = 0, size = total(); i < size; ++i)
        {
            ptr[i] = v;
        }
    }

    float &Tensor::operator[](size_t i)
    {
        return ((float *)m_data)[i];
    }

    const float &Tensor::operator[](size_t i) const
    {
        return ((const float *)m_data)[i];
    }

} // namespace welkin