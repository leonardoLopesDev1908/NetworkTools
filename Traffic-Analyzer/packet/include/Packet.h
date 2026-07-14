#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <string>
#include <utility>

enum IPVersion
{
    v4, v6
};

enum class TransportProtocol
{
    TCP = 1,
    UDP = 2,
    ICMP = 3,
    ICMP6 = 4,
    IGMP = 5,
    UNKNOWN = -1
};

enum class ApplicationProtocol 
{
    HTTP, HTTPS,
    DNS, FTP,
    SSH, SMTP,
    QUIC, NTP,
    UNKNOWN
};

struct Packet
{
    IPVersion ipVersion;
    TransportProtocol transProtocol;
    ApplicationProtocol appProtocol;

    std::string source;
    std::string destiny;

    uint16_t srcPort;
    uint16_t dstPort;
    uint32_t totalLen;
    uint16_t payloadLen;

    const uint8_t* payloadPtr = nullptr;

    Packet(IPVersion ip, TransportProtocol transP, const std::string& src, const std::string& dst,
           int16_t src_port, uint16_t dst_port, uint32_t total_len, uint16_t payload,
           const uint8_t* payload_ptr)
        : ipVersion(ip), transProtocol(transP), source(src), destiny(dst),
          srcPort(src_port), dstPort(dst_port), totalLen(total_len), payloadLen(payload),
          payloadPtr(payload_ptr)
    {
        appProtocol = getApplicationProtocol();
        this->payloadPtr = nullptr;
    }

  private:
    ApplicationProtocol getApplicationProtocol();
};

#endif 
