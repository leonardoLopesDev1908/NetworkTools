#ifndef PLATFORM_H
#define PLATFORM_H

#include <expected>
#include <iostream>

#define NOMINMAX

#ifdef _WIN32
// NOLINTBEGIN(llvm-include-order)
#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
// NOLINTEND(llvm-include-order)

struct ip
{
    uint8_t ip_hl : 4, ip_v : 4;
    uint8_t ip_tos;
    uint16_t ip_len;
    uint16_t ip_id;
    uint16_t ip_off;
    uint8_t ip_ttl;
    uint8_t ip_p;
    uint16_t ip_sum;
    struct in_addr ip_src;
    struct in_addr ip_dst;
};

struct tcphdr {
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;
    uint16_t res1:4,
        doff:4,
        fin:1,
        syn:1,
        rst:1,
        psh:1,
        ack:1,
        urg:1,
        ece:1,
        cwr:1;  
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
};

struct udphdr
{
    uint16_t source;
    uint16_t dest;
    uint16_t len;
    uint16_t check;
};

struct icmphdr
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    union
    {
        struct
        {
            uint16_t id;
            uint16_t sequence;
        } echo;
        uint32_t gateway;
        struct
        {
            uint16_t __unused;
            uint16_t mtu;
        } frag;
    } un;
};

//RFC 2292: https://datatracker.ietf.org/doc/html/rfc2292#section-2.1      
struct ip6_hdr
{
    union
    {
        struct ip6_hdrctl
        {
            uint32_t ip6_un1_flow; /* 24 bits of flow-ID */
            uint16_t ip6_un1_plen; /* payload length */
            uint8_t ip6_un1_nxt;   /* next header */
            uint8_t ip6_un1_hlim;  /* hop limit */
        } ip6_un1;
        uint8_t ip6_un2_vfc; /* 4 bits version, 4 bits priority */
    } ip6_ctlun;
    struct in6_addr ip6_src; /* source address */
    struct in6_addr ip6_dst; /* destination address */
};

#define ip6_nxt ip6_ctlun.ip6_un1.ip6_un1_nxt
struct ip6_ext
{
    uint8_t ip6e_nxt; /* next header.  */
    uint8_t ip6e_len; /* length in units of 8 octets.  */
};

struct ip6_frag
{
    uint8_t ip6f_nxt;      /* next header */
    uint8_t ip6f_reserved; /* reserved field */
    uint16_t ip6f_offlg;   /* offset, reserved, and flag */
    uint32_t ip6f_ident;   /* identification */
};

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
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/icmp6.h>
#include <netinet/igmp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
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
