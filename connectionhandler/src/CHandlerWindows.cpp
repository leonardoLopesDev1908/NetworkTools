#include "CHandlerWindows.h"

#include <string>

CHandlerWindows::CHandlerWindows(SOCKET client, QueueMessage& messages)
	: m_clientSocket(client), m_messages(messages)
{
}

CHandlerWindows::~CHandlerWindows()
{
	stop();
}

void CHandlerWindows::start()
{
	while (receiving)
		read();
}

void CHandlerWindows::read()
{
	char temp[4096];
	Direction direction;
	size_t headerEnd = std::string::npos;

	while (headerEnd == std::string::npos)
	{
		int bytes = recv(m_clientSocket, temp, sizeof(temp), 0);
		if (bytes <= 0) {
			receiving = false;
			break;
		}

		m_buffer.append(temp, bytes);
		headerEnd = m_buffer.find("\r\n\r\n");
	}

	size_t pos = m_buffer.find("Content-Length: ");
	if (pos != std::string::npos)
	{
		int length = std::stoi(m_buffer.substr(pos + 16, m_buffer.find("\r\n", pos)));
		size_t bodyStart = headerEnd + 4;
		size_t currentBodySize = m_buffer.size() - bodyStart;

		while (currentBodySize < length)
		{
			int bytes = recv(m_clientSocket, temp, sizeof(temp), 0);
			if (bytes <= 0) {
				receiving = false;
				break;
			}

			m_buffer.append(temp, bytes);
			currentBodySize += bytes;
		}
	}

	std::string firstLine = m_buffer.substr(0, m_buffer.find("\r\n"));

	if (firstLine.find("HTTP") > 0)
		direction = Direction::Inbound;
	else
		direction = Direction::Outbound;

	m_messages.tryPush(std::move(m_parser.parse(m_buffer, direction)));

	if (direction == Direction::Inbound)
	{
		size_t startServerOrigin = firstLine.find("/");
		std::string destiny = firstLine.substr(
			startServerOrigin,
			firstLine.find("HTTP") - startServerOrigin 
		);
		forward(destiny);
	}
}

void CHandlerWindows::forward(std::string destiny)
{
	size_t sentBytes = 0;
	while (sentBytes < destiny.size())
	{
		int bytes = send(m_clientSocket, destiny.c_str(), sizeof(destiny), 0);
		sentBytes += bytes;
	}
}

void CHandlerWindows::stop()
{
	receiving = false;
	SOCKET s = std::exchange(m_clientSocket, INVALID_SOCKET);
	if (s != INVALID_SOCKET)
		closesocket(s);
}