#pragma once

#include "Common.h"
#include "Condition.h"

template<typename T>
class BlockQueue
{
public:
	BlockQueue()
		: mutex_()
		, notEmpty_(mutex_)
	{
	}

	void put(const T &t)
	{
		MutexLockGuard lock(mutex_);
		queue_.push_back(t);
		notEmpty_.notify();
	}

	T take()
	{
		MutexLockGuard lock(mutex_);
		while (queue_.empty()) {
			notEmpty_.wait();
		}
		assert(!queue_.empty());
		T front(queue_.front());
		queue_.pop_front();
		return front;
	}

	size_t size() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.size(); 
	}

private:
	BlockQueue(const BlockQueue&);
	BlockQueue& operator=(const BlockQueue&);

private:
	mutable MutexLock mutex_;
	Condition notEmpty_;
	std::deque<T> queue_;
};