#ifndef PROXY_LINUX_H
#define PROXY_LINUX_H

#include "CHandlerLinux.h"
#include "QueueMessage.h"
#include "ThreadPool.h"

#include <atomic>
#include <cstring>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class ProxyLinux
{
    int m_socket;
    std::string m_host;
    std::string m_port;

public:
    
    ProxyLinux(std::string host, std::string port);    
    ~ProxyLinux();    
    void create();
    void start();
    void stop();
    QueueMessage& getQueue();

private:
    struct addrinfo hints, * result = nullptr;
    std::atomic<bool> proxyRun = false;
    
    std::mutex m_mutexHandlers;
    std::vector<std::shared_ptr<CHandlerLinux>> m_handlers;
    QueueMessage m_messages;
    ThreadPool m_conns;
};
#endif
