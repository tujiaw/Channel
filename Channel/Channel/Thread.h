#pragma once

#include "Common.h"

class Thread
{
public:
	typedef std::function<void()> ThreadFunc;

	explicit Thread(const ThreadFunc&, const std::string &name=std::string());
	~Thread();

	void start();
	int join();
	bool started() const { return started_; }
	HANDLE handle() const { return handle_; }
	unsigned int tid() const { return tid_; }
	const std::string& name() const { return name_; }

private:
	static unsigned int WINAPI startThread(void *data);
	void runInThread();

private:
	HANDLE handle_;
	unsigned int tid_;
	bool started_;
	ThreadFunc func_;
	std::string name_;
};