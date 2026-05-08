#include "ProxyLinux.h"

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

    int clientSocket = accept(m_socket, result->ai_addr, &result->ai_addrlen);

    //ConnectionHandler newConn(clientSocket); 

    //Como criar o connectionHandler? Dar clientSocket pra ele apenas? Dar addrinfo?
    return 0;
}
