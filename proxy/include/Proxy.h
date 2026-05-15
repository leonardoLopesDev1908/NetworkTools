#ifndef PROXY_H
#define PROXY_H

#include "CHandler.h"
#include "Platform.h"
#include "QueueMessage.h"
#include "ThreadPool.h"

#include <ftxui/component/screen_interactive.hpp>

#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

class Proxy
{
public:
	std::string* m_host = nullptr;
	std::string* m_port = nullptr;
    std::string* m_errorMessage = nullptr;

public:
	Proxy(std::string* host, std::string* port, 
        std::string* errorMessage, ftxui::ScreenInteractive* screen);
    ~Proxy();

	void create();
	void start();
	void stop();
	void keep();
	QueueMessage& getQueue();
	void error(const std::string& errorMessage);
    bool isRunning() const;

private:
    ftxui::ScreenInteractive* m_screen = nullptr;

	SocketType m_socket = INVALID_S;

	bool proxyRun = false;
	std::atomic<bool> keepMessage = false;	
	std::mutex m_handlersMutex;
	std::vector <std::shared_ptr<CHandler>> m_handlers;

	ThreadPool m_conns;
	QueueMessage m_messages;
};

#endif
