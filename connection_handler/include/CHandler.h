#ifndef CHANDLER_H
#define CHANDLER_H

#include "ErrorQueue.h"
#include "HttpParser.h"
#include "Platform.h"
#include "QueueMessage.h"

#include <ftxui/component/screen_interactive.hpp>

#include <atomic>
#include <queue>

class CHandler
{
public:

	CHandler(SocketType&& connection, QueueMessage& messages,
        ErrorQueue& errors);
    ~CHandler();

    void start();
	//void edit();
    std::expected<void, std::string> forwardInbound(std::string destiny, std::string port,
                                                    bool closeClientSocket);
    void forwardOutbound(bool closeClientSocket);
	void stop();

private:
    std::expected<void, std::string> read();
    std::expected<void, std::string> remoteSocket(const std::string& host, 
                                                  const std::string& port);

private:
    std::string m_requestBuf;
    std::string m_responseBuf;

    SocketType m_clientSocket;
    SocketType m_forwardSocket = INVALID_S;

    std::atomic<bool> receiving = true;
	
    ErrorQueue& m_errors;
    HttpParser m_parser;
	QueueMessage& m_messages;
};

#endif
