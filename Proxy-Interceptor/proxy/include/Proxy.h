#ifndef PROXY_H
#define PROXY_H

#include "CHandler.h"
#include "Intercept.h"
#include "Sockets.h"
#include "Queue.h"
#include "ThreadPool.h"

#include <ftxui/component/screen_interactive.hpp>

#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

class Proxy
{
    SocketType m_socket = INVALID_S;
    SocketType iCheck = INVALID_S;

    bool proxyRun = false;
    std::atomic<bool> m_keepMessages;
    std::mutex m_handlersMutex;

    std::vector<std::shared_ptr<CHandler>> m_handlers;

    ThreadPool m_conns;
  
    Queue<std::string> m_errorQueue;
    Queue<Message> m_messages;

public:
    std::string* m_host = nullptr;
    std::string* m_port = nullptr;
    std::string* m_errorMessage = nullptr;
    Intercept m_intercept;

public:
    Proxy(std::string* host, std::string* port, std::string* errorMessage);
    ~Proxy();

    void stop();

    std::expected<void, std::string> create();
    std::expected<void, std::string> start();

    Queue<Message>& getQueue();
    Queue<std::string>& getErrors();

    bool isRunning() const;
    void setKeep(bool keepFlag);
};

#endif
