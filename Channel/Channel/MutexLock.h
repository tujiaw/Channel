#pragma once

#include <WinBase.h>

class MutexLock
{
public:
	MutexLock()
	{
		InitializeCriticalSection(&criticalSection_);
	}

	~MutexLock()
	{
		DeleteCriticalSection(&criticalSection_);
	}

	void lock()
	{
		EnterCriticalSection(&criticalSection_);
	}

	void unlock()
	{
		LeaveCriticalSection(&criticalSection_);
	}

	CRITICAL_SECTION& cs()
	{
		return criticalSection_;
	}

private:
	MutexLock(const MutexLock&);
	MutexLock& operator=(const MutexLock&);

	CRITICAL_SECTION criticalSection_;
};

class MutexLockGuard
{
public:
	explicit MutexLockGuard(MutexLock &mutex)
		: mutex_(mutex)
	{
		mutex_.lock();
	}

	~MutexLockGuard()
	{
		mutex_.unlock();
	}

private:
	MutexLockGuard(const MutexLockGuard&);
	MutexLockGuard operator=(const MutexLockGuard&);

	MutexLock& mutex_;
};
