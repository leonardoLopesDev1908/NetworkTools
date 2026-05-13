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
    //no edit for now
	//void edit();
	void forward(Direction direction);
    void stop();

private: 
    void read();

private:
    std::string m_buffer;
	int m_clientSocket;
    std::atomic<bool> receiving = true;

	HttpParser m_parser;
	QueueMessage& m_messages;
};

#endif
