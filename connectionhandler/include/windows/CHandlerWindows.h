#ifndef CHANDLER_WINDOWS_H
#define CHANDLER_WINDOWS_H

#include "HttpParser.h"
#include "Message.h"

//NOLINTBEGIN(llvm-include-order)
#include <WinSock2.h>  
#include <ws2tcpip.h>   
#include <Windows.h>     
//NOLINTEND(llvm-include-order)

#include <iostream>
#include <queue>

class QueueMessage;

class CHandlerWindows
{
public:

	CHandlerWindows(SOCKET connection, QueueMessage& messages);

	void read();

	//void edit();

	void forward();

private:
	std::string m_buffer;
	SOCKET m_clientSocket;
	HttpParser m_parser;

	QueueMessage& m_messages;
};

#endif