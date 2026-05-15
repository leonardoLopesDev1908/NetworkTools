#ifndef COMMON_UTILITIES_H
#define COMMON_UTILITIES_H

#ifdef _WIN32
    //NOLINTBEGIN(llvm-include-order)
    #include <WinSock2.h>  
    #include <ws2tcpip.h>   
    #include <Windows.h>     
    //NOLINTEND(llvm-include-order)
    
    using SocketType = SOCKET;
    constexpr SocketType INVALID_S = INVALID_SOCKET;

    inline void platformInit()
    {
	    WSADATA wsaData{};
	    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		    throw  std::runtime_error("[Error] start\n");
    }

    inline void platformCleanup() { WSACleanup(); }

    inline void closeSocket(SocketType s) 
    {
        closesocket(s);
    }

    inline bool isInvalid(SocketType s)
    {
        return s == INVALID_SOCKET;
    }

    inline bool isSocketError(SocketType s)
    {
        return s == SOCKET_ERROR;
    }

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
    
    inline void closeSocket(SocketType s) 
    {
        close(s);
    }

    inline bool isInvalid(SocketType s)
    {
        return s == -1;
    }   

    inline bool isSocketError(SocketType s)
    {
        return s == -1;
    }

#endif

#endif
