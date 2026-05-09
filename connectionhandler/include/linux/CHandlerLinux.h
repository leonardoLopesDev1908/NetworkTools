#ifndef CHANDLER_LINUX_H
#define CHANDLER_LINUX_H

#include <iostream>
#include <queue>

class HttpParser;

class CHandlerLinux
{
public:

	CHandlerLinux(int connection);

	void read();

	Message parser(std::string_view data);

    //no edit for now
	//void edit();

	void forward();

private:
    std::string m_buffer;
	int m_clientSocket;

	HttpParser m_parser;
	std::queue<Message> m_messages;
};

#endif
