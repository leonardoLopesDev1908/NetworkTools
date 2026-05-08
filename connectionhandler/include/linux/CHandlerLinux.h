#ifndef CHANDLER_LINUX_H
#define CHANDLER_LINUX_H

#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <queue>

class CHandlerLinux
{
public:

	CHandlerLinux(int connection);

	void read();

	void parser();

	void edit();

	void forward();

private:
	int m_conn;
	HttpParser m_parser;
	std::queue<Message> m_messages;

};

#endif