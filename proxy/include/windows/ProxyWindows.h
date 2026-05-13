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
#include <mutex>
#include <string>
#include <vector>

class ProxyWindows
{
public:
	std::string* m_host = nullptr;
	std::string* m_port = nullptr;
	std::atomic<bool> proxyRun = false;

public:
	ProxyWindows(std::string* host, std::string* port);
	~ProxyWindows();

	void create();
	void start();
	void stop();
	void keep();
	void closeSocket();
	QueueMessage& getQueue();
	void error(const std::string& errorMessage);

private:
	SOCKET m_socket = INVALID_SOCKET;
	WSADATA wsaData{};

	std::atomic<bool> keepMessage = false;	
	std::mutex m_handlersMutex;
	std::vector <std::shared_ptr<CHandlerWindows>> m_handlers;

	ThreadPool m_conns;
	QueueMessage m_messages;
};

#endif
