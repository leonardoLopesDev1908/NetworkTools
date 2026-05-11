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

#include <iostream>
#include <queue>

class CHandlerWindows
{
public:

	CHandlerWindows(SOCKET connection, QueueMessage& messages);
	~CHandlerWindows();

	void start();
	//void edit();
	void forward();
	void closeSocket();

private:
	void read();
	
	bool receiving = true;

	std::string m_buffer;
	SOCKET m_clientSocket;
	HttpParser m_parser;

	QueueMessage& m_messages;
};

#endif