#include "stdafx.h"
#include "Connector.h"

Connector::Connector(const std::string &ip, unsigned short port)
	: ip_(ip)
	, port_(port)
	, func_(nullptr)
	, sock_(INVALID_SOCKET)
	, connected_(false)
{
}

Connector::~Connector(void)
{
	stop();
}

void Connector::start()
{
	connect();
}

void Connector::stop()
{
	connected_ = false;
	if (INVALID_SOCKET != sock_) {
		::shutdown(sock_, 2); // FIXME
		::closesocket(sock_);
		sock_ = INVALID_SOCKET;
	}
}

void Connector::connect()
{
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip_.c_str());
	sin.sin_port = htons(port_);
	sock_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET != sock_) {
		if (0 == ::connect(sock_, (SOCKADDR*)&sin, sizeof(sin))) {
			connected_ = true;
		} else {
			::closesocket(sock_);
			sock_ = INVALID_SOCKET;
		}
	}
}