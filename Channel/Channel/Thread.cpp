#include "stdafx.h"
#include "Thread.h"
#include <process.h>
#include <assert.h>

Thread::Thread(const ThreadFunc &func, const std::string &name)
	: handle_(NULL)
	, tid_(0)
	, started_(false)
	, func_(func)
	, name_(name)
{
}

Thread::~Thread()
{
	CloseHandle(handle_);
}

void Thread::start()
{
	started_ = true;
	handle_ = (HANDLE)_beginthreadex(NULL, 0, startThread, this, 0, &tid_);
	assert(handle_ > 0);
}

int Thread::join()
{
	if (GetCurrentThreadId() == tid_) {
		return 0;
	}
	return WaitForSingleObject(handle_, INFINITE);
}

unsigned int Thread::startThread(void *data)
{
	Thread *thread = static_cast<Thread*>(data);
	thread->runInThread();
	return 0;
}

void Thread::runInThread()
{
	assert(GetCurrentThreadId() == tid_);
	if (func_) {
		func_();
	}
	started_ = false;
}
