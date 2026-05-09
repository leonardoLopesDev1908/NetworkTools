#include "CHandlerLinux.h"

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

CHandlerLinux::CHandlerLinux(int socket)
    : m_clientSocket(socket)
{
}

void CHandlerLinux::read()
{
    while(read(m_clientSocket, m_buffer, 1024) !=0)
    {
        Message msg = parser(m_buffer);
    
        queue.push(msg);
        
        m_buffer.clear();
    }
}

Message CHandlerLinux::parser(std::string_view message)
{
    return m_httpParser.parse(message);
}

void CHandlerLinux::forward()
{

}
