#include "stdafx.h"
#include "Connector.h"
#include "Channel.h"
#include "WSAStartup.h"

#pragma comment(lib, "ws2_32.lib")

class TcpConnection
{
public:
	TcpConnection()
		: connectorPtr_(new Connector("127.0.0.1", 5100))
		, channel_(new Channel(connectorPtr_))
	{
		channel_->setOnReadCallback(std::tr1::bind(&TcpConnection::onRead, this, std::placeholders::_1));
		channel_->setOnErrorCallback(std::tr1::bind(&TcpConnection::onError, this, std::placeholders::_1));
		channel_->setOnErrorCallback(std::tr1::bind(&TcpConnection::onClose, this));
	}

	void start()
	{
		connectorPtr_->start();
		channel_->start();

		char msg[] = "hello,world";
		channel_->send((unsigned char*)msg, strlen(msg));
	}

	~TcpConnection()
	{
	}

private:
	TcpConnection(const TcpConnection&);
	TcpConnection& operator=(const TcpConnection&);

	void onRead(Buffer &buf)
	{
		std::cout << buf.retrieveAllAsString() << std::endl;

		char msg[] = "echo";
		channel_->send((unsigned char*)msg, strlen(msg));
	}

	void onError(int errcode)
	{
	}

	void onClose()
	{
	}

private:
	ConnectorPtr connectorPtr_;
	scoped_ptr<Channel> channel_;
};

int _tmain(int argc, _TCHAR* argv[])
{
	WsaStartup wsaStartup(2, 2);

	TcpConnection tcpConnection;
	tcpConnection.start();

	system("pause");
	return 0;
}
