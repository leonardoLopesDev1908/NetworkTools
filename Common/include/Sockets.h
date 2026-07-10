#if 0
ifndef SOCKETS_H
#define SOCKETS_H

#include "Initialization.h"

#ifdef _WIN32

using SocketType = SOCKET;
constexpr SocketType INVALID_S = INVALID_SOCKET;

inline void closeSocket(SocketType s) { closesocket(s); }

inline bool isInvalid(SocketType s) { return s == INVALID_SOCKET; }

inline bool isSocketError(SocketType s) { return s == SOCKET_ERROR; }


#elif __linux__

#include <sys/socket.h>
#include <unistd.h>

using SocketType = int;
constexpr SocketType INVALID_S = -1;

inline void closeSocket(SocketType s) { close(s); }

inline bool isInvalid(SocketType s) { return s == -1; }

inline bool isSocketError(SocketType s) { return s == -1; }

#endif

#endif
