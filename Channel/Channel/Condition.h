#pragma once

#include "MutexLock.h"

class Condition
{
public:
	explicit Condition(MutexLock &mutex)
		: mutex_(mutex)
	{
		InitializeConditionVariable(&cond_);
	}

	void wait()
	{
		SleepConditionVariableCS(&cond_, &mutex_.cs(), INFINITE);
	}

	void notify()
	{
		WakeConditionVariable(&cond_);
	}

private:
	Condition(const Condition&);
	Condition& operator=(const Condition&);

private:
	MutexLock &mutex_;
	CONDITION_VARIABLE cond_;
};