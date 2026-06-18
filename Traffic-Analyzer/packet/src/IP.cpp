#include "IP.h"
#include "Platform.h" 

#include <array>
#include <cstdio>
#include <stdexcept>

const std::string& IP::getSource() const { return src; }
const std::string& IP::getDestiny() const { return dst; }
TransportProtocol IP::getProtocol() const { return transport; }
uint16_t IP::getPayloadLen() const { return payloadLen; }

IPv4::IPv4(const uint8_t* data)
    : ipHdr(reinterpret_cast<const ip*>(data)), ipHdrLen(static_cast<uint8_t>(ipHdr->ip_hl) * 4)
{
#ifdef _WIN32
    auto result = platformInit();
    if (!result.has_value()) 
        throw std::runtime_error(result.error());
#endif

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
}

uint16_t IPv4::getSourcePort() const { return srcPort; }
uint16_t IPv4::getDestinyPort() const { return destPort; }

void IPv4::handleTcp() {}
void IPv4::handleUdp() {}
void IPv4::handleIcmp() {}
void IPv4::handleIcmp6() {}
void IPv4::handleIgmp() {}