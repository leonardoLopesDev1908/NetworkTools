#ifndef CHANDLER_H
#define CHANDLER_H

#include "Platform.h"
#include "HttpParser.h"
#include "QueueMessage.h"

#include <ftxui/component/screen_interactive.hpp>

#include <atomic>
#include <iostream>
#include <queue>

class CHandler
{
public:
    std::string* m_errorMessage = nullptr;

public:

	CHandler(SocketType&& connection, QueueMessage& messages, const std::string* port,
            std::string* errorMessage, ftxui::ScreenInteractive* screen);
    ~CHandler();
    void start();
	//void edit();
	void forwardInbound(std::string destiny, bool closeClientSocket);
	void forwardOutbound(bool closeClientSocket);
	void stop();

private:
	void read();
	void remoteSocket(const std::string& destiny);
    void error(std::string message);

private:
    ftxui::ScreenInteractive* m_screen;

    const std::string* m_port;

    std::string m_requestBuf;
    std::string m_responseBuf;

    SocketType m_clientSocket;
    SocketType m_forwardSocket = INVALID_S;

    std::atomic<bool> receiving = true;

	HttpParser m_parser;
	QueueMessage& m_messages;
};

#endif
