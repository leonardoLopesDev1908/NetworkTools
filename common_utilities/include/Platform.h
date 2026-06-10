#ifndef COMMON_UTILITIES_H
#define COMMON_UTILITIES_H

#include <expected>
#include <iostream>

#define NOMINMAX

#ifdef _WIN32
// NOLINTBEGIN(llvm-include-order)
#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
// NOLINTEND(llvm-include-order)

using SocketType = SOCKET;
constexpr SocketType INVALID_S = INVALID_SOCKET;

inline std::expected<void, std::string> platformInit()
{
    WSADATA wsaData{};
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return std::unexpected("[Error] start\n");
    return {};
}

inline void platformCleanup() { WSACleanup(); }

inline void closeSocket(SocketType s) { closesocket(s); }

inline bool isInvalid(SocketType s) { return s == INVALID_SOCKET; }

inline bool isSocketError(SocketType s) { return s == SOCKET_ERROR; }

#elif __linux__
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

using SocketType = int;
constexpr SocketType INVALID_S = -1;

inline void platformInit() {}
inline void platformCleanup() {}

inline void closeSocket(SocketType s) { close(s); }

inline bool isInvalid(SocketType s) { return s == -1; }

inline bool isSocketError(SocketType s) { return s == -1; }

#endif

#endif
