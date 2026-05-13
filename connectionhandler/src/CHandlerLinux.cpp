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
    Direction direction;
    size_t headerEnd = std::string::npos;
    
    while(headerEnd == std::string::npos)
    {
        int bytes = recv(m_clientSocket, temp, sizeof(temp), 0);
        if(bytes <= 0)
        {
            receiving = false;
            break;
        }

        m_buffer.append(temp, bytes);
        headerEnd = m_buffer.find("\r\n\r\n");
    }

    size_t pos = m_buffer.find("Content-Length");
    if(pos != std::string::npos)
    {
        int bodySize = std::stoi(m_buffer.substr(pos + 16, m_buffer.find("\r\n", pos)));
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

            m_buffer.append(temp, bytes);
            currentBodySize += bytes;
        }
    }
    
    std::string firstLine = m_buffer.substr(0, m_buffer.find("\r\n"));

    if(firstLine.find("HTTP") > 0)
        direction = Direction::Inbound;
    else
        direction = Direction::Outbound;

    m_messages.tryPush(std::move(m_parser.parse(m_buffer, direction)));

    
    std::string destiny;
    if(direction == Direction::Outbound)
    {
        size_t startPath = firstLine.find("/");

    }
        
    m_buffer.clear();
}

void CHandlerLinux::stop()
{
    int s = std::exchange(m_clientSocket, -1);
    if(s != -1)
        close(s);
}

void CHandlerLinux::forward(Direction direction)
{
    size_t sentBytes = 0;
    while(sentBytes < m_buffer.size())
    {
        int bytes = send(m_clientSocket, destiny.c_str(), m_buffer.size(), 0);

        sentBytes += bytes;
    }
}
