#include "ProxyWindows.h"

ProxyWindows::ProxyWindows(std::string host, std::string port)
	: m_host(host), m_port(port) 
{	
}

ProxyWindows::~ProxyWindows()
{
	m_messages.setScreen(nullptr);
	if (m_socket != INVALID_SOCKET)
		closesocket(m_socket);
	WSACleanup();
}

void ProxyWindows::create()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		throw  std::runtime_error("[Error] start\n");
	}

	struct addrinfo* result = nullptr;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int iResult = getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &result);

	if (iResult != 0)
	{
		throw std::runtime_error("[Error] getaddrinfo: " + std::to_string(iResult));
	}

	m_socket = INVALID_SOCKET;
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_socket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		throw std::runtime_error("[Error] invalid socket\n");
	}

	if (bind(m_socket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		closesocket(m_socket);
		throw std::runtime_error("[Error] binding socket\n");
	}
	
	if(result != nullptr)
		freeaddrinfo(result);

}

void ProxyWindows::start()
{
	create();

	if (listen(m_socket, 5) == SOCKET_ERROR)
	{
		throw std::runtime_error("[Error] listening socket\n");
	}

	proxyRun = true;

	while (proxyRun)
	{
		SOCKET client = INVALID_SOCKET;
		if ((client = accept(m_socket, NULL, NULL)) == INVALID_SOCKET)
		{
			throw std::runtime_error("[Error] accepting socket");
		}

		//if (keepMessage)
		//{
			//asynchronous launch to edit message
			// 
			//ConnectionHandler newConn(client); 
		//}
		
		auto newConn = std::make_shared<CHandlerWindows>(client, m_messages);
		
		m_conns.enqueue([conn = std::move(newConn)](){
			conn->start();
		});
	}
}

void ProxyWindows::keep()
{
	keepMessage = (keepMessage == false) ? true : false;
}

void ProxyWindows::stop()
{
	proxyRun = false;
	if(m_socket != INVALID_SOCKET)
		closesocket(m_socket);
	WSACleanup();
}

void ProxyWindows::closeSocket()
{
	SOCKET s = std::exchange(m_socket, INVALID_SOCKET);
	if (s != INVALID_SOCKET)
		closesocket(s);
}

QueueMessage& ProxyWindows::getQueue()
{
	return m_messages;
}