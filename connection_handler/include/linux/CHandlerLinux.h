#ifndef CHANDLER_LINUX_H
#define CHANDLER_LINUX_H

#include "HttpParser.h"
#include "QueueMessage.h"

#include <atomic>
#include <iostream>
#include <queue>

class CHandlerLinux
{
public:

	CHandlerLinux(int connection, QueueMessage& m_messages);
    ~CHandlerLinux();
    void start();
	//void edit();
	void forwardInbound(std::string destiny);
	void forwardOutbound(bool closeClientSocket);
	void closeSocket();
	void stop();

private:
	void read();
	void remoteSocket(const std::string& destiny);

private:
    std::string m_requestBuf;
    std::string m_responseBuf;

	int m_clientSocket;
    int m_forwardSocket;

    std::atomic<bool> receiving = true;

	HttpParser m_parser;
	QueueMessage& m_messages;
};

#endif
