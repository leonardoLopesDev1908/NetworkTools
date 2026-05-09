#include "CHandlerWindows.h"

CHandlerWindows::CHandlerWindows(SOCKET client, QueueMessage& messages)
	: m_clientSocket(client), m_messages(messages)
{
}

void CHandlerWindows::read()
{
	while (recv(m_clientSocket, m_buffer.data(), 1024, 0) != 0)
	{
		//Include verification to know the direction
		Direction direction = Direction::Inbound;

		Message msg = m_parser.parse(m_buffer, direction);


		m_buffer.clear();
	}
}

void CHandlerWindows::forward()
{

}
