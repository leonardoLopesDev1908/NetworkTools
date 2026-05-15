#include "CHandlerWindows.h"

#include <fstream>
#include <string>

CHandlerWindows::CHandlerWindows(SOCKET&& client, QueueMessage& messages, std::string& port)
	: m_clientSocket(std::move(client)), m_messages(messages), m_port(port)
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
	size_t headerEnd = std::string::npos;

	while (headerEnd == std::string::npos)
	{
		int bytes = recv(m_clientSocket, temp, sizeof(temp), 0);
		if (bytes <= 0) {
			receiving = false;
			break;
		}

		m_requestBuf.append(temp, bytes);
		headerEnd = m_requestBuf.find("\r\n\r\n");
	}

	size_t pos = m_requestBuf.find("Content-Length: ");
	if (pos != std::string::npos)
	{
		int length = std::stoi(m_requestBuf.substr(
                    pos + 16, m_requestBuf.find("\r\n", pos) - pos
        ));

		size_t bodyStart = headerEnd + 4;
		size_t currentBodySize = m_requestBuf.size() - bodyStart;

		while (currentBodySize < length)
		{
			int bytes = recv(m_clientSocket, temp, sizeof(temp), 0);
			if (bytes <= 0) {
				receiving = false;
				break;
			}

			m_requestBuf.append(temp, bytes);
			currentBodySize += bytes;
		}
	}

	std::string firstLine = m_requestBuf.substr(0, m_requestBuf.find("\r\n"));

	Message msg = m_parser.parse(m_requestBuf, Direction::Inbound);
	
    std::string destiny = msg.headers["Host"];
    bool closeClientSocket = false;

    if(msg.headers["Connection"] == "close")
        closeClientSocket = true;

	forwardInbound(destiny, closeClientSocket);

	m_messages.tryPush(std::move(msg));
}

void CHandlerWindows::forwardInbound(std::string destiny, bool closeClientSocket)
{
	size_t sentBytes = 0;
	remoteSocket(destiny);

	while (sentBytes < m_requestBuf.size())
	{
		int bytes = send(m_forwardSocket, m_requestBuf.c_str(), m_requestBuf.size(), 0);
		sentBytes += bytes;
	}

	m_requestBuf.clear();

    forwardOutbound(closeClientSocket);
}

void CHandlerWindows::forwardOutbound(bool closeClientSocket)
{
	char response[4096];
	size_t receivedBytes;
	while (true)
	{
		int bytes = recv(m_forwardSocket, response, sizeof(response), 0);
		if (bytes <= 0) break;
		m_responseBuf.append(response, bytes);
	}

    size_t sentBytes = 0;
	while (sentBytes < m_responseBuf.size())
	{
		int bytes = send(m_clientSocket, m_responseBuf.c_str(), m_responseBuf.size(), 0);
		if(bytes <= 0) break;

        sentBytes += bytes;
	}

    Message msg = m_parser.pse(m_responseBuf, Direction::Outbound);

    if(msg.headers["Connection"] == "close")
        closesocket(m_forwardSocket);
    
    if(closeClientSocket)
    {
        receiving = false;
    }

    m_messages.tryPush(std::move(msg));

	m_responseBuf.clear();
}

//when to close this socket?
void CHandlerWindows::remoteSocket(const std::string& hostDestiny)
{
	struct addrinfo hints{}, *result = nullptr;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(hostDestiny.c_str(), m_port.c_str(), &hints, &result) != 0)
	{
		throw std::runtime_error("[Error]: getaddrinfo forward socket\n");
	}

	m_forwardSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_forwardSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		throw std::runtime_error("[Error]: socket forward socket\n");
	}

	if (connect(m_forwardSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		throw std::runtime_error("[Error]: connect forward socket\n");
	}

	freeaddrinfo(result);
}

void CHandlerWindows::stop()
{
	receiving = false;
	SOCKET s = std::exchange(m_clientSocket, INVALID_SOCKET);
	if (s != INVALID_SOCKET)
		closesocket(s);

	SOCKET f = std::exchange(m_forwardSocket, INVALID_SOCKET);
	if (f != INVALID_SOCKET)
		closesocket(f);
}

