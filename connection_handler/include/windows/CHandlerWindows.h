#ifndef CHANDLER_WINDOWS_H
#define CHANDLER_WINDOWS_H

#include "HttpParser.h"
#include "Message.h"
#include "QueueMessage.h"

//NOLINTBEGIN(llvm-include-order)
#include <WinSock2.h>  
#include <ws2tcpip.h>   
#include <Windows.h>     
//NOLINTEND(llvm-include-order)

#include <atomic>
#include <iostream>
#include <queue>

class CHandlerWindows
{
public:
	CHandlerWindows(SOCKET&& connection, QueueMessage& messages, std::string& port);
	~CHandlerWindows();

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
	std::atomic<bool> receiving = true;
	std::string& m_port;

	std::string m_responseBuf;
	std::string m_requestBuf;

	SOCKET m_clientSocket;
	SOCKET m_forwardSocket = INVALID_SOCKET;
	HttpParser m_parser;

	QueueMessage& m_messages;
};

#endif
