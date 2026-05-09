#ifndef PROXY_WINDOWS_H
#define PROXY_WINDOWS_H

#define NOMINMAX

#include "CHandlerWindows.h"
#include "QueueMessage.h"
#include "ThreadPool.h"

//NOLINTBEGIN(llvm-include-order)
#include <WinSock2.h>  
#include <ws2tcpip.h>   
#include <Windows.h>     
//NOLINTEND(llvm-include-order)

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ProxyWindows
{
	SOCKET m_socket = INVALID_SOCKET;
	WSADATA wsaData{};

	std::string m_host;
	std::string m_port;

public:
	ProxyWindows(std::string host, std::string port);
	~ProxyWindows();

	int create();
	int start();
	void stop();
	void keep();

private:
	std::atomic<bool> proxyRun = false;
	std::atomic<bool> keepMessage = false;

	ThreadPool m_conns;
	QueueMessage m_messages;
};

#endif
