#include "allocator.h"

namespace welkin
{
    PoolAllocator::PoolAllocator() : d(new PoolAllocatorPrivate)
    {
        d->size_compare_ratio = 192; // 0.75f * 256
    }

    PoolAllocator::~PoolAllocator()
    {
        // clear budgets
        clear();

        if (!d->payouts.empty())
        {
            WELKIN_LOGE("FATAL ERROR! pool allocator destroyed too early");
#ifdef WELKIN_STDIO
            std::list<std::pair<size_t, void *>>::iterator it = d->payouts.begin();
            for (; it != d->payouts.end(); ++it)
            {
                void *ptr = it->second;
                WELKIN_LOGE("%p still in use", ptr);
            }
#endif
            delete d;
        }
    }

    void PoolAllocator::clear()
    {
        // clear budgets
        d->budgets_lock.lock();

        std::list<std::pair<size_t, void*>>::iterator it = d->budgets.begin();
        for (; it != d->budgets.end(); ++it)
        {
            void *ptr = it->second;
            fastFree(ptr);
        }
        d->budgets.clear();

        d->budgets_lock.unlock();
    }

    void PoolAllocator::set_size_compare_ratio(float scr)
    {
        if (scr < 0.f || scr > 1.f)
        {
            WELKIN_LOGE("invalid size compare ratio %f", scr);
            return;
        }

        d->size_compare_ratio = (unsigned int)(scr * 256);
    }

    void *PoolAllocator::fastMalloc(size_t size)
    {
        // find free budget
        d->budgets_lock.lock();

        std::list<std::pair<size_t, void*>>::iterator it = d->budgets.begin();
        for (; it != d->budgets.end(); ++it)
        {
            size_t bs = it->first;
            if (bs >= size && (bs * d->size_compare_ratio) >> 8 <= size)
            {
                void *ptr = it->second;
                d->budgets.erase(it);
                d->budgets_lock.unlock();

                d->payouts_lock.lock();
                d->payouts.push_back(std::make_pair(bs, ptr));
                d->payouts_lock.unlock();

                return ptr;
            }
        }

        d->budgets_lock.unlock();

        // new
        void *ptr = fastMalloc(size);
        d->payouts_lock.lock();
        d->payouts.push_back(std::make_pair(size, ptr));
        d->payouts_lock.unlock();

        return ptr;
    }

    void PoolAllocator::fastFree(void *ptr)
    {
        d->payouts_lock.lock();

        // return to budgets
        std::list<std::pair<size_t, void*>>::iterator it = d->payouts.begin();
        for (; it != d->payouts.end(); ++it)
        {
            if (ptr == it->second)
            {
                size_t bs = it->first;
                d->payouts.erase(it);
                d->payouts_lock.unlock();

                d->budgets_lock.lock();
                d->budgets.push_back(std::make_pair(bs, ptr));
                d->budgets_lock.unlock();

                return;
            }
        }

        d->payouts_lock.unlock();
        WELKIN_LOGE("FATAL ERROR! pool allocator get wild pointer: %p", ptr);
        fastFree(ptr);
    }
}