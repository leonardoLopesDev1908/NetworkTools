#include "ProxyWindows.h"

ProxyWindows::ProxyWindows(std::string host, std::string port)
	: m_host(host), m_port(port) {
	
}

ProxyWindows::~ProxyWindows()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
	}
	WSACleanup();
}

int ProxyWindows::create()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "[Error] start\n";
		return 1;
	}

	struct addrinfo* result = nullptr;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &result) != 0)
	{
		std::cout << "[Error] getaddrinfo\n";
		WSACleanup();
		return 1;
	}

	m_socket = INVALID_SOCKET;
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_socket == INVALID_SOCKET)
	{
		std::cout << "[Error] invalid socket\n";
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	if (bind(m_socket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		std::cout << "[Error] binding socket\n";
		freeaddrinfo(result);
		closesocket(m_socket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);
	return 0;
}

int ProxyWindows::start()
{
	if (create() != 0)
	{
		//throw std::err...
		return 1;
	}

	if (listen(m_socket, 5) == SOCKET_ERROR)
	{
		std::cout << "[Error] listening socket\n";
		closesocket(m_socket);
		WSACleanup();
		return 1;
	}

	proxyRun = true;

	while (proxyRun)
	{
		SOCKET client = INVALID_SOCKET;
		if ((client = accept(client, NULL, NULL)) == INVALID_SOCKET)
		{
			std::cout << "[Error] accepting\n";
			closesocket(m_socket);
			WSACleanup();
			return 1;
		}

		if (keepMessage)
		{
			//asynchronous launch to edit message
			// 
			//ConnectionHandler newConn(client); 
		}
		else
		{
			//just keep at 
		}

	}
	return 0;
}

void ProxyWindows::keep()
{
	keepMessage = (keepMessage == false) ? true : false;
}

void ProxyWindows::stop()
{
	proxyRun = false;
	closesocket(m_socket);
	WSACleanup();
}