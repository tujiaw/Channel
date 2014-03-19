#include "stdafx.h"
#include "Channel.h"
#include "Thread.h"
#include "Connector.h"

Channel::Channel(ConnectorPtr connectorPtr)
	: connectorPtr_(connectorPtr)
	, sendQueue_(new BlockQueue<BufferPtr>())
{
	recvBufferThread_.reset(new Thread(std::bind(&Channel::recvBufferThread, this), "eventHandleThread"));
	sendBufferThread_.reset(new Thread(std::bind(&Channel::sendBufferThread, this), "sendBufferThread"));
}

Channel::~Channel(void)
{
}

void Channel::start()
{
	connectorPtr_->start();
	recvBufferThread_->start();
	sendBufferThread_->start();
}

void Channel::stop()
{
	connectorPtr_->stop();
	BufferPtr lastBufferPtr(new Buffer());
	sendQueue_->put(lastBufferPtr);

	recvBufferThread_->join();
	sendBufferThread_->join();

	sendQueue_.reset(new BlockQueue<BufferPtr>());
}

void Channel::send(const unsigned char *msg, size_t len)
{
	BufferPtr bufferPtr(new Buffer());
	bufferPtr->append(msg, len);
	sendQueue_->put(bufferPtr);
}

void Channel::send(const BufferPtr &msg)
{
	sendQueue_->put(msg);
}

void Channel::sendBufferThread()
{
	std::cout << "recvBufferThread" << std::endl;
	while (connectorPtr_->connected()) {
		BufferPtr bufferPtr = sendQueue_->take();
		if (!connectorPtr_->connected()) {
			break;
		}

		unsigned int left = bufferPtr->readableBytes();
		while (left > 0) {
			int sendNum = ::send(connectorPtr_->socket(), bufferPtr->peek(), left, 0);
			if (SOCKET_ERROR == sendNum) {
				onErrorCallback_(TCPCONN_ERROR_SEND);
				break;
			}
			left -= sendNum;
			bufferPtr->retrieve(sendNum);
		}
	}

	std::cout << "recvBufferThread exit" << std::endl;
}

void Channel::recvBufferThread()
{
	std::cout << "sendBufferThread" << std::endl;
	while (connectorPtr_->connected()) {
		static const int bufferLen = 65535;;
		char buffer[bufferLen];
		int recvNum = ::recv(connectorPtr_->socket(), buffer, bufferLen, 0);
		if (recvNum > 0) {
			recvBuffer_.append(buffer, recvNum);
			onReadCallback_(recvBuffer_);
		} else if (0 == recvNum) {
			onCloseCallback_();
			break;
		} else {
			onErrorCallback_(TCPCONN_ERROR_RECV);
			break;
		}
	}

	std::cout << "sendBufferThread exit" << std::endl;
}

