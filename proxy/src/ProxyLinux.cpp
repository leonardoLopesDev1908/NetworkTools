#include "ProxyLinux.h"

ProxyLinux::ProxyLinux(std::string host, std::string port, 
        std::vector<std::unique_ptr<CHandlerLinux>>& conns)
    : m_conns(conns), m_host(host), m_port(port)
{
}

int ProxyLinux::create()
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);    

    if(m_socket != 0)
    {
        printf("[Error] socket\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &result)
        != 0) {
        std::cout << "Erro no getaddrinfo" << std::endl;
        close(m_socket);
        return 1;
    }

    if (bind(m_socket, result->ai_addr,
        (int)result->ai_addrlen)) {
        std::cout << "Erro no bind" << std::endl;
        close(m_socket);
        return 1;
    }

    return 0;
}

int ProxyLinux::start()
{
    if(create() == 1)
    {
        //throw error
        return 1;
    }

    listen(m_socket, 5);

    proxyRun = true;
    
    while(proxyRun)
    {
        int clientSocket;

        if((clientSocket = accept(m_socket, result->ai_addr, &result->ai_addrlen)) != 0)
        {
            std::cerr << "[Error] accepting connection\n";
            continue;
        }

        //if(keepMessages)
        //{
           //ConnectionHandler newConn(clientSocket);
           
        //} else
        
        CHandlerLinux newConn(clientSocket);
    
        //Thred Pool para handlers 
        std::thread t(newConn->start());
        m_conns.push_back(newConn);
    }

    return 0; 
}
