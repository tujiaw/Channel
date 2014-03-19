#pragma once

#include "Common.h"

class Connector
{
public:
	typedef std::function<void(SOCKET fd)> ConnectionCallback;

	Connector(const std::string &ip, unsigned short port);
	virtual ~Connector(void);
	
	virtual void start();
	virtual void stop();

	void setConnectionCallback(const ConnectionCallback &func) { func_ = func; }
	SOCKET socket() const { return sock_; }
	bool connected() const { return connected_; }

private:
	void connect();

private:
	std::string ip_;
	unsigned short port_;
	ConnectionCallback func_;
	SOCKET sock_;
	bool connected_;
};
