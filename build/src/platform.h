#ifndef WELKIN_PLATFORM_H
#define WELKIN_PLATFORM_H

#include "welkin_export.h"

#ifdef __cplusplus
#if (defined _WIN32 && !(defined __MINGW32__))
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif // (defined _WIN32 && !(defined __MINGW32__))

namespace welkin
{
#if (defined _WIN32 && !(defined __MINGW32__))
    class WELKIN_EXPORT Mutex
    {
    public:
        Mutex() { InitializeSRWLock(&srwlock); }
        ~Mutex() {}
        void lock() { AcquireSRWLockExclusive(&srwlock); }
        void unlock() { ReleaseSRWLockExclusive(&srwlock); }

    private:
        friend class ConditionVariable;
        // NOTE SRWLock is available from windows vista
        SRWLOCK srwlock;
    };

    class WELKIN_EXPORT ConditionVariable
    {
    public:
        ConditionVariable() { InitializeConditionVariable(&condvar); }
        ~ConditionVariable() {}
        void wait(Mutex &mutex) { SleepConditionVariableSRW(&condvar, &mutex.srwlock, INFINITE, 0); }
        void broadcast() { WakeAllConditionVariable(&condvar); }
        void signal() { WakeConditionVariable(&condvar); }

    private:
        CONDITION_VARIABLE condvar;
    };

    static unsigned __stdcall start_wrapper(void *args);
    class WELKIN_EXPORT Thread
    {
    public:
        Thread(void *(*start)(void *), void *args = 0)
        {
            _start = start;
            _args = args;
            handle = (HANDLE)_beginthreadex(0, 0, start_wrapper, this, 0, 0);
        }

        ~Thread() {}

        void join()
        {
            WaitForSingleObject(handle, INFINITE);
            CloseHandle(handle);
        }

    private:
        friend unsigned __stdcall start_wrapper(void *args)
        {
            Thread *t = (Thread *)args;
            t->_start(t->_args);
            return 0;
        }

        HANDLE handle;

        void *(*_start)(void *);

        void *_args;
    };

    class WELKIN_EXPORT ThreadLocalStorage
    {
    public:
        ThreadLocalStorage() { key = TlsAlloc(); }
        ~ThreadLocalStorage() { TlsFree(key); }
        void set(void *value) { TlsSetValue(key, (LPVOID)value); }
        void *get() { return (void *)TlsGetValue(key); }

    private:
        DWORD key;
    };

#else  // (defined _WIN32 && !(defined __MINGW32__))
    class WELKIN_EXPORT Mutex
    {
    public:
        Mutex() { pthread_mutex_init(&mutex, 0); }
        ~Mutex() { pthread_mutex_destroy(&mutex); }
        void lock() { pthread_mutex_lock(&mutex); }
        void unlock() { pthread_mutex_unlock(&mutex); }

    private:
        friend class ConditionVariable;
        pthread_mutex_t mutex;
    };

    class WELKIN_EXPORT ConditionVariable
    {
    public:
        ConditionVariable() { pthread_cond_init(&cond, 0); }
        ~ConditionVariable() { pthread_cond_destroy(&cond); }
        void wait(Mutex &mutex) { pthread_cond_wait(&cond, &mutex.mutex); }
        void broadcast() { pthread_cond_broadcast(&cond); }
        void signal() { pthread_cond_signal(&cond); }

    private:
        pthread_cond_t cond;
    };

    class WELKIN_EXPORT Thread
    {
    public:
        Thread(void *(*start)(void *), void *args = 0) { pthread_create(&t, 0, start, args); }
        ~Thread() {}
        void join() { pthread_join(t, 0); }

    private:
        pthread_t t;
    };

    class WELKIN_EXPORT ThreadLocalStorage
    {
    public:
        ThreadLocalStorage() { pthread_key_create(&key, 0); }
        ~ThreadLocalStorage() { pthread_key_delete(key); }
        void set(void *value) { pthread_setspecific(key, value); }
        void *get() { return pthread_getspecific(key); }

    private:
        pthread_key_t key;
    };
#endif // (defined _WIN32 && !(defined __MINGW32__))
    class WELKIN_EXPORT MutexLockGuard
    {
    public:
        MutexLockGuard(Mutex &_mutex) : mutex(_mutex) { mutex.lock(); }
        ~MutexLockGuard() { mutex.unlock(); }

    private:
        Mutex &mutex;
    };
}

// include need head file
#include <algorithm>
#include <list>
#include <vector>
#include <string>

#endif // __cplusplus

#ifdef WELKIN_STDIO
#define WELKIN_LOGE(...)                \
    {                                   \
        fprintf(stderr, ##__VA_ARGS__); \
        fprintf(stderr, "\n");          \
    }
#else
#define WELKIN_LOGE(...)
#endif

#endif // WELKIN_PLATFORM_H
