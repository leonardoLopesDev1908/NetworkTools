#include "IP.h"

#include <array>
#include <cstdio>
#include <stdexcept>

const std::string& IP::getSource() const { return src; }
const std::string& IP::getDestiny() const { return dst; }

TransportProtocol IP::getProtocol() const { return transport; }

uint16_t IP::getPayloadLen() const { return payloadLen; }

IPv4::IPv4(const uint8_t* data)
    : ipHdr(reinterpret_cast<const ip*>(data)), 
    ipHdrLen(static_cast<uint8_t>(ipHdr->ip_hl) * 4)
{
    if (ipHdrLen < 20)
    {
        throw std::runtime_error("Load ip failed: header length too short");
    }

    std::array<char, INET_ADDRSTRLEN> srcBuf{};
    inet_ntop(AF_INET, &(ipHdr->ip_src), srcBuf.data(), srcBuf.size());
    src = srcBuf.data();

    std::array<char, INET_ADDRSTRLEN> dstBuf{};
    inet_ntop(AF_INET, &(ipHdr->ip_dst), dstBuf.data(), dstBuf.size());
    dst = dstBuf.data();

    switch (ipHdr->ip_p)
    {
        case IPPROTO_TCP:
            IPv4::handleTcp();
            break;
        case IPPROTO_UDP: 
            IPv4::handleUdp();
            break;
        case IPPROTO_ICMP:
            IPv4::handleIcmp();
            break;
        case IPPROTO_ICMPV6:
            IPv4::handleIcmp6();
            break;
        case IPPROTO_IGMP:
            IPv4::handleIgmp();
            break;
        default:
            transport = TransportProtocol::UNKNOWN;
            return;
    }
}

uint16_t IPv4::getSourcePort() { return srcPort; }
uint16_t IPv4::getDestinyPort() { return destPort; }
uint8_t IPv4::getHeaderLen() const { return ipHdrLen; }

void IPv4::handleTcp() 
{  
    const auto *tcp = reinterpret_cast<const tcphdr*>(
        reinterpret_cast<const uint8_t*>(ipHdr) + ipHdrLen
    );

    srcPort = ntohs(tcp->source);
    destPort = ntohs(tcp->dest);

    const auto tcpHdrLen = static_cast<std::size_t>(tcp->doff) * 4U;
    
    payloadPtr = reinterpret_cast<const uint8_t*>(tcp) + tcpHdrLen;
    payloadLen = static_cast<uint16_t>(ntohs(ipHdr->ip_len)) - ipHdrLen - static_cast<int>(tcpHdrLen);

    transport = TransportProtocol::TCP;
}

void IPv4::handleUdp()
{
    const auto *udp = reinterpret_cast<const udphdr*>(
        reinterpret_cast<const uint8_t*>(ipHdr) + ipHdrLen
    );

    srcPort = ntohs(udp->source);
    destPort = ntohs(udp->dest);

    if (static_cast<std::size_t>(udp->len) >= 8)
    {
        payloadPtr = reinterpret_cast<const uint8_t*>(udp) + 8;
        payloadLen = static_cast<uint16_t>(ntohs(udp->len)) - 8;
    }

    transport = TransportProtocol::UDP;
}

void IPv4::handleIcmp() { transport = TransportProtocol::ICMP; }

void IPv4::handleIcmp6() { transport = TransportProtocol::ICMP6; }

void IPv4::handleIgmp() { transport = TransportProtocol::IGMP; }

// ---- IPv6 ----
IPv6::IPv6(const uint8_t* data) 
    : ipHdr(reinterpret_cast<const ip6_hdr*>(data)),
      packetPtr(reinterpret_cast<const uint8_t*>(ipHdr + 1))
{
    uint8_t hdr = ipHdr->ip6_ctlun.ip6_un1.ip6_un1_nxt;

    std::array<char, INET6_ADDRSTRLEN> srcBuf{};
    inet_ntop(AF_INET6, &ipHdr->ip6_src, srcBuf.data(), sizeof(srcBuf));
    src = srcBuf.data();
    ipSource = ipHdr->ip6_src;

    std::array<char, INET6_ADDRSTRLEN> dstBuf{};
    inet_ntop(AF_INET6, &ipHdr->ip6_dst, dstBuf.data(), sizeof(dstBuf));
    dst = dstBuf.data();
    ipDest = ipHdr->ip6_dst;

    while (true)
    {
        switch (hdr)
        {
        case IPPROTO_TCP:
            IPv6::handleTcp();
            return;
        case IPPROTO_UDP:
            IPv6::handleUdp();
            return;
        case IPPROTO_ICMP:
            IPv6::handleIcmp();
            return;
        case IPPROTO_ICMPV6:
            IPv6::handleIcmp6();
            return;
        case IPPROTO_IGMP:
            IPv6::handleIgmp();
            return;

        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_DSTOPTS:
        {
            const auto* ext = reinterpret_cast<const ip6_ext*>(packetPtr);
            hdr = ext->ip6e_nxt;
            packetPtr += (static_cast<std::size_t>(ext->ip6e_len) + 1U) * 8U; //advance pointer
            ipExtLen += (static_cast<std::size_t>(ext->ip6e_len) + 1U) * 8U; //save extension header length
            break;
        }
        case IPPROTO_FRAGMENT:
            const auto* frag = reinterpret_cast<const ip6_frag*>(packetPtr);
            hdr = frag->ip6f_nxt;
            packetPtr += sizeof(ip6_frag);
            ipExtLen += sizeof(ip6_frag);
            break;
        }
    }

    packetPtr = nullptr;
}

uint16_t IPv6::getSourcePort() { return srcPort; }

uint16_t IPv6::getDestinyPort() { return destPort; }

void IPv6::handleTcp()
{
    /* if (packetPtr + sizeof(tcphdr) > bufferEnd) 
    //Later: receive the end of buffer by constructor
    {
        printf("Malformed TCP header\n");
        packetPtr = nullptr;
        return;
    }*/
    const auto* tcp = reinterpret_cast<const tcphdr *>(packetPtr);
    const auto tcpHdrLen = static_cast<std::size_t>(tcp->doff) * 4U; 
    //ntohs is not necessary here because doff has less than one byte size

    srcPort = ntohs(tcp->source);
    destPort = ntohs(tcp->dest);

    //if (!(packetPtr + tcpHdrLen + > packetEnd))
    auto plen = ntohs(ipHdr->ip6_ctlun.ip6_un1.ip6_un1_plen);

    payloadPtr = reinterpret_cast<const uint8_t *>(tcp) + tcpHdrLen;  
    payloadLen = static_cast<uint16_t>(plen) - tcpHdrLen;

    transport = TransportProtocol::TCP;
    packetPtr = nullptr;
}

void IPv6::handleUdp() 
{
    /* if (packetPtr + sizeof(udphdr) > bufferEnd)
    //Later: receive the end of buffer by constructor
    {
        printf("Malformed UDP header\n");
        packetPtr = nullptr;
        return;
    }*/
    const auto* udp = reinterpret_cast<const udphdr *>(packetPtr);

    srcPort = ntohs(udp->source);
    destPort = ntohs(udp->dest);

    if(ntohs(udp->len) >= 8U)
    {
        payloadPtr = reinterpret_cast<const uint8_t *>(packetPtr) + 8U;
        payloadLen = static_cast<uint16_t>(
            ntohs(ipHdr->ip6_ctlun.ip6_un1.ip6_un1_plen)
        ) - 8U;
    }

    transport = TransportProtocol::UDP;
    packetPtr = nullptr;
}

void IPv6::handleIcmp() 
{
    transport = TransportProtocol::ICMP;
    packetPtr = nullptr;
}

void IPv6::handleIcmp6() 
{
    transport = TransportProtocol::ICMP6;
    payloadLen = ntohs(ipHdr->ip6_ctlun.ip6_un1.ip6_un1_plen) - sizeof(icmp6_hdr);
    packetPtr = nullptr;
}

void IPv6::handleIgmp()
{  
    transport = TransportProtocol::IGMP;
    packetPtr = nullptr;
}
