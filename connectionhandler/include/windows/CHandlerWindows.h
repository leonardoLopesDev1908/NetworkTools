#ifndef CHANDLER_WINDOWS_H
#define CHANDLER_WINDOWS_H

#include "HttpParser.h"

//NOLINTBEGIN(llvm-include-order)
#include <WinSock2.h>  
#include <ws2tcpip.h>   
#include <Windows.h>     
//NOLINTEND(llvm-include-order)

#include <queue>

class CHandlerWindows
{
public:

	CHandlerWindows(SOCKET connection);

	void read();

	void parser();

	void edit();

	void forward();

private:
	SOCKET m_conn;
	HttpParser m_parser;
	std::queue<Message> m_messages;

};

#endif