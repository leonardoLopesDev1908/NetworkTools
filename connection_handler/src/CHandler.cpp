#include "CHandler.h"

#include <cstring>
#include <fstream>
#include <string>
#include <utility>


CHandler::CHandler(SocketType&& clientSocket, Queue<Message>& messages, 
				Queue<std::string>& errors, Intercept& intercept, std::atomic<bool>* keepFlag)
	: m_clientSocket(std::move(clientSocket)), m_messages(messages), m_errors(errors), 
	m_intercept(intercept), m_keep(keepFlag)
{
}

CHandler::~CHandler()
{
	stop();
}

void CHandler::start()
{
	while (receiving)
	{
		auto result = read();
		if (!result)
		{
			m_errors.tryPush(std::move(result.error()));
		}
	}
}

std::expected<void, std::string> CHandler::read()
{
	char temp[4096];
	size_t headerEnd = std::string::npos;

	while (headerEnd == std::string::npos)
	{
		int bytes = recv(m_clientSocket, temp, sizeof(temp), 0);
		if (bytes <= 0) {
			receiving = false;
			return {};
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
	std::string host = destiny;
	std::string port = "80";

	size_t portPos = destiny.find(":");
	if (portPos != std::string::npos)
	{
		host = destiny.substr(0, portPos);
		port = destiny.substr(portPos + 1);
	}

	bool closeClientSocket = false;
    if(msg.headers["Connection"] == "close")
        closeClientSocket = true;
	
	if (*m_keep)
	{
		auto edited = m_intercept.wait(std::move(msg));
        if(edited)
            msg = std::move(*edited);
	}

	m_messages.tryPush(std::move(msg));
	
	auto result = forwardInbound(host, port, closeClientSocket);

	if (!result)
		return result;

	return {};
}

std::expected<void, std::string> CHandler::forwardInbound(std::string host, std::string port,
											bool closeClientSocket)
{
	size_t sentBytes = 0;
	
	auto result = remoteSocket(host, port);

	if (!result)
		return result;

	while (sentBytes < m_requestBuf.size())
	{
		int bytes = send(m_forwardSocket, m_requestBuf.c_str(), m_requestBuf.size(), 0);
		sentBytes += bytes;
	}

	m_requestBuf.clear();

    forwardOutbound(closeClientSocket);

	return {};
}

void CHandler::forwardOutbound(bool closeClientSocket)
{
	char response[4096];
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

	size_t headerEnd = m_responseBuf.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
	{
		m_responseBuf.clear();
		return;
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

std::expected<void, std::string> CHandler::remoteSocket(const std::string& host,
														const std::string& port)
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

	int iCheck = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
	if (iCheck != 0)
	{
	    return std::unexpected("[Error]: getaddrinfo forward socket: " + 
			std::to_string(iCheck) + "\n");
	}

	m_forwardSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	
    if (m_forwardSocket == INVALID_S)
    {
		freeaddrinfo(result);
		return std::unexpected("[Error]: socket forward socket: " + 
			std::to_string(m_forwardSocket) + '\n');
	}

	iCheck = connect(m_forwardSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iCheck != 0)
    {
		freeaddrinfo(result);
		return std::unexpected("[Error]: connect forward socket: " + 
			std::to_string(iCheck) + "\n");
	}

	freeaddrinfo(result);
	return {};
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

