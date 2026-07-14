#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "Initialization.h"

#ifdef _WIN32

struct ether_header
{
    u_char ether_dhost[6];
    u_char ether_shost[6];
    u_short ether_type;
};

constexpr uint16_t ETHERTYPE_IP = 0x0800;   
constexpr uint16_t ETHERTYPE_IPV6 = 0x86DD; 
//constexpr uint16_t ETHERTYPE_ARP = 0x0806;  


// https:github.com/leostratus/netinet/blob/master/icmp6.h
struct icmp6_hdr
{
    uint8_t icmp6_type;   /* type field */
    uint8_t icmp6_code;   /* code field */
    uint16_t icmp6_cksum; /* checksum field */
    union
    {
        uint32_t icmp6_un_data32[1]; /* type-specific field */
        uint16_t icmp6_un_data16[2]; /* type-specific field */
        uint8_t icmp6_un_data8[4];   /* type-specific field */
    } icmp6_dataun;
};

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

// RFC 2292: https://datatracker.ietf.org/doc/html/rfc2292#section-2.1
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

// Struct for IPv6 header extensions (HOPOPTS, ROUTING DSTOPTS)
// Ptr arith -> (ptr + 1) + 8
struct ip6_ext
{
    uint8_t ip6e_nxt; /* next header.  */
    uint8_t ip6e_len; /* length in units of 8 octets.*/
};

// Struct for IPv6 header extension FRAGMENT
// Ptr arith -> sizeof(ip6_frag) as this headers has 8 bytes
struct ip6_frag
{
    uint8_t ip6f_nxt;      /* next header */
    uint8_t ip6f_reserved; /* reserved field */
    uint16_t ip6f_offlg;   /* offset, reserved, and flag */
    uint32_t ip6f_ident;   /* identification */
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

#elif __linux__

#include <netinet/icmp6.h>
#include <netinet/if_ether.h>
#include <netinet/igmp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#endif
#endif
