#include "Proxy.h"

#include <cstring>
#include <utility>

SocketType iCheck = INVALID_S;


Proxy::Proxy(std::string* host, std::string* port, 
        std::string* errorMessage, ftxui::ScreenInteractive* screen)
	: m_host(host), m_port(port), m_errorMessage(errorMessage),
    m_screen(screen)
{	
}

Proxy::~Proxy()
{
	stop();
}

void Proxy::create()
{
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
		error("[Error] getaddrinfo: " + std::to_string(iResult));
	}

	m_socket = INVALID_S;
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_socket == INVALID_S)
	{
		freeaddrinfo(result);
		error("[Error] invalid socket\n");
	}

    iCheck = bind(m_socket, result->ai_addr, (int)result->ai_addrlen);
	
    if (isSocketError(iCheck))
	{
		freeaddrinfo(result);
		error("[Error] binding socket\n");
	}

	if (result != nullptr)
		freeaddrinfo(result);
}

void Proxy::start()
{
	create();

    iCheck = listen(m_socket, 5);

	if (isSocketError(iCheck))
	{
		error("[Error] listening socket\n");
	}

	proxyRun = true;

	while (proxyRun)
	{
		SocketType clientSocket = INVALID_S;
		if ((clientSocket = accept(m_socket, NULL, NULL)) == INVALID_S)
		{
			error("[Error] accepting socket");
		}

		//if (keepMessage)
		//{
			//asynchronous launch to edit message
			// 
			//ConnectionHandler newConn(client); 
		//}
		
		//SOCKET forwardSocket = createSocket();
		//Pass


		auto newConn = std::make_shared<CHandler>(std::move(clientSocket), m_messages, m_port,
                 m_errorMessage, m_screen);
		 {
		 	std::lock_guard<std::mutex> lck(m_handlersMutex);
		 	m_handlers.push_back(newConn);
		 }
		 m_conns.enqueue([conn = newConn, this](){
		 	try{
		 		conn->start();
		 	}catch(const std::exception& e)
		 	{
		 		std::cerr << e.what() << std::endl;
		 		stop();
		 	}
		 });
	}
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

QueueMessage& Proxy::getQueue()
{
	return m_messages;
}

void Proxy::error(const std::string& errorMessage)
{
	m_host->clear();
	m_port->clear();
	stop();
	throw std::runtime_error(errorMessage);
}

bool Proxy::isRunning() const
{
    return proxyRun;
}
