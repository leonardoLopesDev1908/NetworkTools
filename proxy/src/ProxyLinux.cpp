#include "ProxyLinux.h"

#include <memory>

ProxyLinux::ProxyLinux(std::string host, std::string port) 
    : m_host(host), m_port(port)
{
}

ProxyLinux::~ProxyLinux()
{
    stop();
}

void ProxyLinux::create()
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);    

    if(m_socket < 0)
    {
        throw std::runtime_error("[Error] invalid socket\n");
    }

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &result) < 0) {
        freeaddrinfo(result);
        throw std::runtime_error("[Error] getaddrinfo\n");
    }

    if (bind(m_socket, result->ai_addr, (int)result->ai_addrlen) < 0) {
        freeaddrinfo(result);
        throw std::runtime_error("[Error]: binding socket\n");
    }

    freeaddrinfo(result);
}

void ProxyLinux::start()
{
    create();

    if(listen(m_socket, 5) < 0)
    {
        throw std::runtime_error("[Error] listening...\n");
    }

    proxyRun = true;
    
    while(proxyRun)
    {
        int clientSocket;

        if((clientSocket = accept(m_socket, NULL, NULL)) < 0)
        {
            if(!proxyRun) break;
            throw std::runtime_error("[Error] accepting connection\n");
        }

        //if(keepMessages)
        //{
           //ConnectionHandler newConn(clientSocket);
           
        //} else
        
        auto newConn = std::make_shared<CHandlerLinux>(clientSocket, m_messages);
    
        m_conns.enqueue([c = newConn]{
            c->start();
        });

        m_handlers.push_back(newConn);
    }
}

void ProxyLinux::stop()
{
    proxyRun = false;
    
    int s = std::exchange(m_socket, -1);
    if(s != -1)
        close(s);

    {
        std::lock_guard<std::mutex> lck(m_mutexHandlers);
        for(auto& h : m_handlers)
            h->stop(); 
    }

    //m_poll.stop();
}

QueueMessage& ProxyLinux::getQueue()
{
    return m_messages;
}
