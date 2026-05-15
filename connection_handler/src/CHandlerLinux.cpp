#include "CHandlerLinux.h"

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

CHandlerLinux::CHandlerLinux(int socket, QueueMessage& messages)
    : m_clientSocket(socket), m_messages(messages)
{
}

CHandlerLinux::~CHandlerLinux()
{
    stop();
}

void CHandlerLinux::start()
{
    while(receiving)
        read();
}

void CHandlerLinux::read()
{
    char temp[4096];
    size_t headerEnd = std::string::npos;
    
    while(headerEnd == std::string::npos)
    {
        int bytes = recv(m_clientSocket, temp, sizeof(temp), 0);
        if(bytes <= 0)
        {
            receiving = false;
            break;
        }

        m_requestBuf.append(temp, bytes);
        headerEnd = m_requestBuf.find("\r\n\r\n");
    }

    size_t pos = m_requestBuf.find("Content-Length");
    if(pos != std::string::npos)
    {
        int bodySize = std::stoi(m_requestBuf.substr(
                    pos + 16, m_requestBuf.find("\r\n", pos) - pos
        ));

        size_t bodyStart = headerEnd + 4;
        size_t currentBodySize = 0;
 
        while(currentBodySize < bodySize)
        {
            int bytes = recv(m_clientSocket, temp, sizeof(temp), 0);
            if(bytes <= 0)
            {
                receiving = false;
                break;
            }

            m_requestBuf.append(temp, bytes);
            currentBodySize += bytes;
        }
    }
    
    std::string firstLine = m_requestBuf.substr(0, m_requestBuf.find("\r\n"));

    Message msg = m_parser.parse(m_requestBuf, Direction::Inbound);

    bool closeClientSocket = false;
    std::string destiny;
    
    if(msg.headers["Connection"] == "close")
        closeClientSocket = true;

	forwardInbound(destiny, closeClientSocket);


    m_messages.tryPush(std::move(msg));
}

void CHandlerLinux::forwardInbound(std::string& destiny, bool closeClientSocket)
{
    size_t sentBytes = 0;
    remoteSocket(destiny);
    while(sentBytes < m_requestBuf.size())
    {
        int bytes = send(m_clientSocket, destiny.c_str(), m_requestBuf.size(), 0);
        if(bytes <= 0) break;

        sentBytes += bytes;
    }

    m_requestBuf.clear();

    forwardOutbound(closeClientSocket);
}

void CHandlerLinux::forwardOutbound(bool closeClientSocket)
{
    char response[4096];


}

void CHandlerLinux::stop()
{
    int s = std::exchange(m_clientSocket, -1);
    if(s != -1)
        close(s);
}
