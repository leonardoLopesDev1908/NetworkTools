#include "Proxy.h"

#include <cstring>
#include <utility>

Proxy::Proxy(std::string* host, std::string* port, 
    std::string* errorMessage)
	: m_host(host), m_port(port), m_errorMessage(errorMessage)
{	
}

Proxy::~Proxy()
{
	stop();
}

std::expected<void, std::string> Proxy::create()
{

#ifdef _WIN32
    auto initResult = platformInit();
	if (!initResult)
		return initResult;
#endif

	struct addrinfo* result = nullptr;
	struct addrinfo hints;

#ifdef _WIN32
	ZeroMemory(&hints, sizeof(hints));
#elif __linux__
    memset(&hints, 0, sizeof(hints));
#endif

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int iResult = getaddrinfo(m_host->c_str(), m_port->c_str(), &hints, &result);

	if (iResult != 0)
	{
		return std::unexpected("[Error] getaddrinfo: " + std::to_string(iResult));
	}

	m_socket = INVALID_S;
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_socket == INVALID_S)
	{
		freeaddrinfo(result);
		return std::unexpected("[Error] invalid socket\n");
	}

    iCheck = bind(m_socket, result->ai_addr, (int)result->ai_addrlen);
	
    if (isSocketError(iCheck))
	{
		freeaddrinfo(result);
		return std::unexpected("[Error] binding socket\n");
	}

	if (result != nullptr)
		freeaddrinfo(result);

	return {};
}

std::expected<void, std::string> Proxy::start()
{
	auto result = create();

	if (!result)
	{
		return result;
	}

    iCheck = listen(m_socket, 5);

	if (isSocketError(iCheck))
	{
		return std::unexpected("[Error] listening socket\n");
	}

	proxyRun = true;

	while (proxyRun)
	{
		SocketType clientSocket = INVALID_S;
		if ((clientSocket = accept(m_socket, NULL, NULL)) == INVALID_S)
		{
			if (!proxyRun) return {};
			return std::unexpected("[Error] accepting socket");
		}

		auto newConn = std::make_shared<CHandler>(std::move(clientSocket), m_messages,
                 m_errorQueue, m_intercept, &m_keepMessages);
		{
			std::lock_guard<std::mutex> lck(m_handlersMutex);
			m_handlers.push_back(newConn);
		}

		m_conns.enqueue([conn = newConn, this](){
			conn->start();
		});
	}

	return {};
}

void Proxy::stop()
{
	m_messages.setScreen(nullptr);
	proxyRun = false;
	
	SocketType s = std::exchange(m_socket, INVALID_S);
	if(s != INVALID_S)
		closeSocket(s);

	{
		std::lock_guard<std::mutex>lck(m_handlersMutex);
		for (auto& handler : m_handlers)
			handler->stop();
		m_handlers.clear();
	}

	m_conns.stop();
	platformCleanup();
}

Queue<Message>& Proxy::getQueue()
{
	return m_messages;
}

Queue<std::string>& Proxy::getErrors()
{
	return m_errorQueue;
}

bool Proxy::isRunning() const
{
    return proxyRun;
}

void Proxy::setKeep(bool keepFlag)
{
	m_keepMessages = keepFlag;
}
