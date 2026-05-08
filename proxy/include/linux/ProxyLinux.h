#ifndef PROXY_LINUX_H
#define PROXY_LINUX_H
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class ProxyLinux
{
    int m_socket;
    std::string m_host;
    std::string m_port;

public:
    
    ProxyLinux(std::string host, std::string port);    
    ProxyLinux(const ProxyLinux& copy) = delete;
    ProxyLinux(ProxyLinux&& move) = delete;

    int create();
    int start();
    void stop();

private:

    struct addrinfo hints, * result = nullptr;
    bool proxyRun = false;
};
#endif