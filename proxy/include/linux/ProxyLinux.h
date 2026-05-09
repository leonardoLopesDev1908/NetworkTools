#ifndef PROXY_LINUX_H
#define PROXY_LINUX_H

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
    
    ProxyLinux(std::string host, std::string port,
            std::vector<std::unique_ptr<CHandlerLinux>>& conns);    
    ProxyLinux(const ProxyLinux& copy) = delete;
    ProxyLinux(ProxyLinux&& move) = delete;

    int create();
    int start();
    void stop();

private:
   
    std::vector<std::unique_ptr<CHandlerLinux>>& m_conns;    

    struct addrinfo hints, * result = nullptr;
    std::atomic<bool> proxyRun = false;
};
#endif
