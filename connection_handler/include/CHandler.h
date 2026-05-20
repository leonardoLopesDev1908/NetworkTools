#ifndef CHANDLER_H
#define CHANDLER_H

#include "Intercept.h"
#include "HttpParser.h"
#include "Platform.h"
#include "Queue.h"

#include <ftxui/component/screen_interactive.hpp>

#include <atomic>
#include <queue>

class CHandler
{
public:

    CHandler(SocketType&& connection, Queue<Message>& messages,
        Queue<std::string>& errors, Intercept& intercept, std::atomic<bool>* keepFlag);
    ~CHandler();

    void start();
    std::expected<void, std::string> forwardInbound(std::string destiny, std::string port,
                                                    bool closeClientSocket);
    void forwardOutbound(bool closeClientSocket);
	void stop();
    void cancel();

private:
    std::expected<void, std::string> read();
    std::expected<void, std::string> remoteSocket(const std::string& host, 
                                                  const std::string& port);

private:
    std::atomic<bool>* m_keep = nullptr;

    std::string m_requestBuf;
    std::string m_responseBuf;

    SocketType m_clientSocket;
    SocketType m_forwardSocket = INVALID_S;

    std::atomic<bool> receiving = true;
	
    Intercept& m_intercept;
    HttpParser m_parser;
    Queue<std::string>& m_errors;
	Queue<Message>& m_messages;
};

#endif
