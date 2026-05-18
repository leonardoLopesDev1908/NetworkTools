#ifndef PROXY_H
#define PROXY_H

#include "CHandler.h"
#include "ErrorQueue.h"
#include "Platform.h"
#include "QueueMessage.h"
#include "ThreadPool.h"

#include <ftxui/component/screen_interactive.hpp>

#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

class Proxy
{
public:
	std::string* m_host = nullptr;
	std::string* m_port = nullptr;
    std::string* m_errorMessage = nullptr;

public:
	Proxy(std::string* host, std::string* port, 
        std::string* errorMessage);
    ~Proxy();

	std::expected<void, std::string> create();
	std::expected<void, std::string> start();
	void stop();
	//void keep();
	QueueMessage& getQueue();
	ErrorQueue& getErrors();
    bool isRunning() const;

private:
	SocketType m_socket = INVALID_S;
	SocketType iCheck = INVALID_S;

	bool proxyRun = false;
	std::atomic<bool> keepMessage = false;	
	std::mutex m_handlersMutex;
	
	std::vector <std::shared_ptr<CHandler>> m_handlers;

	ErrorQueue m_errorQueue;
	ThreadPool m_conns;
	QueueMessage m_messages;
};

#endif
