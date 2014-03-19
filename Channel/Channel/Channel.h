#pragma once

#include "Buffer.h"
#include "BlockQueue.h"

#define TCPCONN_SUCCESS							0
#define TCPCONN_ERROR_BASE						0x00100
#define TCPCONN_ERROR_SOCKET					TCPCONN_ERROR_BASE + 1
#define TCPCONN_ERROR_CONNECT					TCPCONN_ERROR_BASE + 2
#define TCPCONN_ERROR_SEND						TCPCONN_ERROR_BASE + 3
#define TCPCONN_ERROR_RECV						TCPCONN_ERROR_BASE + 4
#define TCPCONN_ERROR_WSAEVENTSELECT			TCPCONN_ERROR_BASE + 5
#define TCPCONN_ERROR_WSAENUMNETWORKEVENTS		TCPCONN_ERROR_BASE + 6
#define TCPCONN_ERROR_WSAWAITFORMULTIPLEEVENTS	TCPCONN_ERROR_BASE + 7


typedef std::function<void(Buffer&)> OnReadCallback;
typedef std::function<void(int)> OnErrorCallback;
typedef std::function<void()> OnCloseCallback;

class Connector;
class Thread;
template<typename T> class BlockQueue;
typedef std::shared_ptr<Connector> ConnectorPtr;

class Channel
{
public:
	Channel(ConnectorPtr connectorPtr);
	~Channel();

	void start();
	void stop();

	void send(const unsigned char *msg, size_t len);
	void send(const BufferPtr &msg);

	void setOnReadCallback(const OnReadCallback &cb) { onReadCallback_ = cb; }
	void setOnErrorCallback(const OnErrorCallback &cb) { onErrorCallback_ = cb; }
	void setOnCloseCallback(const OnCloseCallback &cb) { onCloseCallback_ = cb; }
	
private:
	Channel(const Channel&);
	Channel& operator=(const Channel&);

	void sendBufferThread();
	void recvBufferThread();

private:
	ConnectorPtr connectorPtr_;
	scoped_ptr<Thread> recvBufferThread_;
	scoped_ptr<Thread> sendBufferThread_;
	scoped_ptr<BlockQueue<BufferPtr> > sendQueue_;
	Buffer recvBuffer_;

	OnReadCallback onReadCallback_;
	OnErrorCallback onErrorCallback_;
	OnCloseCallback onCloseCallback_;
};
