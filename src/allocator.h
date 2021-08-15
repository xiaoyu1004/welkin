#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstdlib>

#include "platform.h"

namespace welkin
{
// the alignment of all the allocated buffers
#define MALLOC_ALIGN 16

    template <typename T>
    static inline T *alignPtr(T *ptr, int n = (int)sizeof(T))
    {
        return (T *)(((size_t)ptr + n - 1) & -n);
    }

    static inline int alignSize(size_t sz, int n)
    {
        return (sz + n - 1) & -n;
    }

    static inline void *fastMalloc(size_t size)
    {
#if _MSC_VER
        return _aligned_malloc(size, MALLOC_ALIGN);
#elif
        unsigned char *udata = malloc(size + sizeof(void *) + MALLOC_ALIGN);
        if (!udata)
            return nullptr;
        unsigned char **adata = alignPtr((unsigned char **)udata + 1, MALLOC_ALIGN);
        adata[-1] = udata;
        return adata;
#endif
    }

    static inline void fastFree(void *ptr)
    {
#if _MSC_VER
        _aligned_free(ptr);
#elif
        unsigned char *udata = ((unsigned char **)ptr)[-1];
        free(udata);
#endif
    }

    static inline int WELKIN_XADD(int *addr, int delta)
    {
        int tmp = *addr;
        *addr += delta;
        return tmp;
    }

    class WELKIN_EXPORT Allocator
    {
    public:
        virtual ~Allocator();
        virtual void *fastMalloc(size_t size);
        virtual void fastFree(void *ptr);
    };

    class PoolAllocatorPrivate
    {
    public:
        Mutex budgets_lock;
        Mutex payouts_lock;
        unsigned int size_compare_ratio; // 0~256
        std::list<std::pair<size_t, void *>> budgets;
        std::list<std::pair<size_t, void *>> payouts;
    };

    class WELKIN_EXPORT PoolAllocator : public Allocator
    {
    public:
        PoolAllocator();
        ~PoolAllocator();
        
    public:
        PoolAllocator(const PoolAllocator &) = delete;
        PoolAllocator &operator=(const PoolAllocator &) = delete;

    public:
        // ratio range 0 ~ 1
        // default cr = 0.75
        void set_size_compare_ratio(float scr);

        // release all budgets immediately
        void clear();

        virtual void *fastMalloc(size_t size);
        virtual void fastFree(void *ptr);

    private:
        PoolAllocatorPrivate *const d;
    };
}

#endif