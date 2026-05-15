#include "CHandler.h"

#include <cstring>
#include <fstream>
#include <string>
#include <utility>

CHandler::CHandler(SocketType&& clientSocket, QueueMessage& messages, const std::string* port,
       std::string* errorMessage, ftxui::ScreenInteractive* screen)
	: m_clientSocket(std::move(clientSocket)), m_messages(messages), m_port(port),
    m_errorMessage(errorMessage), m_screen(screen)
{
}

CHandler::~CHandler()
{
	stop();
}

void CHandler::start()
{
	while (receiving)
		read();
}

void CHandler::read()
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

void CHandler::forwardInbound(std::string destiny, bool closeClientSocket)
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

void CHandler::forwardOutbound(bool closeClientSocket)
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

    Message msg = m_parser.parse(m_responseBuf, Direction::Outbound);

    if(msg.headers["Connection"] == "close")
        closeSocket(m_forwardSocket);
    
    if(closeClientSocket)
    {
        receiving = false;
    }

    m_messages.tryPush(std::move(msg));

	m_responseBuf.clear();
}

void CHandler::remoteSocket(const std::string& hostDestiny)
{
	struct addrinfo hints{}, *result = nullptr;
	
#ifdef _WIN32
    ZeroMemory(&hints, sizeof(hints));
#elif __linux__
    memset(&hints, 0, sizeof(hints));
#endif

    hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(hostDestiny.c_str(), "80", &hints, &result) != 0)
	{
	    error("[Error]: getaddrinfo forward socket\n");
	}

	m_forwardSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	
    if (m_forwardSocket == INVALID_S)
    {
		freeaddrinfo(result);
		error("[Error]: socket forward socket\n");
	}

    if (connect(m_forwardSocket, result->ai_addr, (int)result->ai_addrlen) != 0)
    {
		freeaddrinfo(result);
		error("[Error]: connect forward socket\n");
	}

	freeaddrinfo(result);
}

void CHandler::stop()
{
	receiving = false;
	SocketType s = std::exchange(m_clientSocket, INVALID_S);
	if (s != INVALID_S)
		closeSocket(s);

	SocketType f = std::exchange(m_forwardSocket, INVALID_S);
	if (f != INVALID_S)
		closeSocket(f);
}

void CHandler::error(std::string message)
{
//    *m_errorMessage = message;

    if (m_screen)
        m_screen->PostEvent(ftxui::Event::Custom);
    throw std::runtime_error(message);
}
