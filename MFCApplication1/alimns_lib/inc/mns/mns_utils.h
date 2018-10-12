// Copyright (C) 2015, Alibaba Cloud Computing

#ifndef MNS_SDK_UTILS_H
#define MNS_SDK_UTILS_H

#include <string>

#ifdef _WIN32
#include <memory>
#include <windows.h>
#else
#include <tr1/memory>
#include <sys/time.h>
#include <string.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include "mns_exception.h"
#include "mns_common_tool.h"

namespace mns
{
namespace sdk
{

#define MNS_LOCK_SAFE(x) \
    do                      \
    {                       \
        int r = (x);        \
        if (r != 0)         \
        {                   \
            int e = errno;  \
            std::cerr << "MNS_LOCK_SAFE to abort() @ " << __FILE__ << ":" <<__LINE__ << " in " << __FUNCTION__ << std::endl \
                      << "Ret:" << r << " errno:" << e << "(" << strerror(e) << ')' << std::endl; \
            abort();        \
        }                   \
    } while(false)

	class PTMutex
	{
	public:
		PTMutex()
		{
#ifdef _WIN32
			InitializeCriticalSection(&cs);
#else
			MNS_LOCK_SAFE(pthread_mutex_init(&mutex, NULL));
#endif
		}

		~PTMutex()
		{
#ifdef _WIN32
			DeleteCriticalSection(&cs);
#else
			MNS_LOCK_SAFE(pthread_mutex_destroy(&mutex));
#endif
		}

		void lock()
		{
#ifdef _WIN32
			EnterCriticalSection(&cs);
#else
			MNS_LOCK_SAFE(pthread_mutex_lock(&mutex));
#endif
		}

		void unlock()
		{
#ifdef _WIN32
			LeaveCriticalSection(&cs);
#else
			MNS_LOCK_SAFE(pthread_mutex_unlock(&mutex));
#endif
		}

#ifdef _WIN32
		CRITICAL_SECTION& get_pthread_mutex()
		{
			return cs;
		}
#else
		pthread_mutex_t& get_pthread_mutex()
		{
			return mutex;
		}
#endif

	protected:
#ifdef _WIN32
		CRITICAL_SECTION cs;
#else
		mutable pthread_mutex_t mutex;
#endif
	};

class PTCond
{
public:
    PTCond()
    {
#ifdef _WIN32
		InitializeConditionVariable(&cond);
#else
        MNS_LOCK_SAFE(pthread_cond_init(&cond, NULL));
#endif
    }

    ~PTCond()
    {
#ifndef _WIN32
        MNS_LOCK_SAFE(pthread_cond_destroy(&cond));
#endif
    }

    int wait(PTMutex& mutex, int64_t usec)
    {
#ifdef _WIN32
		if (usec < 0)
		{
			if (SleepConditionVariableCS(&cond, &(mutex.get_pthread_mutex()), INFINITE))
			{
				return 0;
			}
			else
			{
				return GetLastError();
			}
		}
		else
		{
#define MICROSECONDS_PER_MILLISECOND 1000
			if (SleepConditionVariableCS(&cond, &(mutex.get_pthread_mutex()), usec / MICROSECONDS_PER_MILLISECOND))
			{
				return 0;
			}
			else
			{
				return GetLastError();
			}
#undef MICROSECONDS_PER_MILLISECOND
		}

#else

		int ret;

        if (usec == -1)
        {
            // never fail
            ret = pthread_cond_wait(&cond, &mutex.get_pthread_mutex());
        }
        else
        {
            if (usec < 0)
            {
                MNS_THROW(MNSExceptionBase, "PTCond::wait only accepts positve usec (expect usec = -1 for infinity wait)");
            }
            timespec ts;
            timeval tp;
            gettimeofday(&tp, 0);

#define MICROSECONDS_PER_SECOND 1000000
#define NANOSECONDS_PER_MICROSECONDS 1000

            /// Calculate "second" part. Caution, usec may exceeds MICROSECONDS_PER_SECOND.
            ts.tv_sec = tp.tv_sec + (usec / MICROSECONDS_PER_SECOND);
            /// Calcuate "microsecond" part.
            int usec_sum = tp.tv_usec + usec % MICROSECONDS_PER_SECOND;

            /// Adjust "second" part if usec_sum exceeds MICROSECONDS_PER_SECOND
            ts.tv_sec += usec_sum / MICROSECONDS_PER_SECOND;
            usec_sum %= MICROSECONDS_PER_SECOND;

            /// Calcuate nanosecond
            ts.tv_nsec = usec_sum * NANOSECONDS_PER_MICROSECONDS;
#undef MICROSECONDS_PER_SECOND
#undef NANOSECONDS_PER_MICROSECONDS
            // fail if timed-out or interupted by a signal
            ret = pthread_cond_timedwait(&cond, &mutex.get_pthread_mutex(), &ts);
        }

        return ret;
#endif
    }

    void signal()
    {
#ifdef _WIN32
		WakeConditionVariable(&cond);
#else
        MNS_LOCK_SAFE(pthread_cond_signal(&cond));
#endif
    }

protected:
#ifdef _WIN32
	CONDITION_VARIABLE cond;
#else
    pthread_cond_t cond;
#endif
};

class WaitObject
{
    friend class PTScopedLock;

    PTMutex mutex;
    PTCond cond;
public:
    WaitObject() {}
    int wait(int64_t timeout = -1)
    {
        return cond.wait(mutex, timeout);
    }
    void signal()
    {
        cond.signal();
    }
};

typedef std::tr1::shared_ptr<WaitObject> WaitObjectPtr;

class PTScopedLock
{
private:
    PTMutex& mDevice;
    PTScopedLock(const PTScopedLock&);
    PTScopedLock& operator=(const PTScopedLock&);

public:
    // lock when construct
    explicit PTScopedLock(PTMutex& m) : mDevice(m)
    {
        mDevice.lock();
    }
    explicit PTScopedLock(WaitObject& wo) : mDevice(wo.mutex)
    {
        mDevice.lock();
    }

    // unlock when destruct
    ~PTScopedLock()
    {
        mDevice.unlock();
    }
};

class MNSUtils
{
public:
    static std::string getLastNodeFromURL(const std::string& inputURL);
    static std::string escapeJson(const std::string &s);
    static std::string toJsonStr(const std::map<std::string, std::string>& param);
};

}
}

#endif
