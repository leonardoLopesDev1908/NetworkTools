#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <expected>

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>

inline std::expected<void, std::string> platformInit()
{
    WSADATA wsaData{};
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return std::unexpected("[Error] start\n");
    return {};
}

inline void platformCleanup() { WSACleanup(); }

#elif __linux__

#include <arpa/inet.h>
#include <netdb.h>

inline void platformInit() {}
inline void platformCleanup() {}

#endif

#endif
