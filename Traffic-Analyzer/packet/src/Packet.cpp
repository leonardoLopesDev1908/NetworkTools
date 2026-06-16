#include "Packet.h"
#include <cstring>

ApplicationProtocol Packet::getApplicationProtocol()
{
    if (payloadPtr != nullptr && payloadLen >= 4)
    {
        if (transProtocol == TransportProtocol::TCP)
        {
            if (memcmp(payloadPtr, "GET ", 4) == 0 || memcmp(payloadPtr, "HEAD", 4) == 0
                || memcmp(payloadPtr, "POST", 4) == 0 || memcmp(payloadPtr, "HEAD", 4) == 0 ||
                memcmp(payloadPtr, "HTTP", 4) == 0)
            {
                return ApplicationProtocol::HTTP;
            }
        }

        if ((srcPort == 53 || dstPort == 53) && payloadLen >= 12)
        {
            //DNS requires by standard the mininum header size of 12 bytes
            return ApplicationProtocol::DNS;
        }

        if (transProtocol == TransportProtocol::TCP)
        {
            //For HTTPS, the first bytes 0x16 means the handshake
            //and the second byte as 0x03 means the TLS version supported
            if (payloadPtr[0] == 0x16 && payloadPtr[1] == 0x03)
            {
                return ApplicationProtocol::HTTPS;
            }
        }
    }

    if(transProtocol == TransportProtocol::ICMP 
        || transProtocol == TransportProtocol::ICMP6 
        || transProtocol == TransportProtocol::IGMP)
    {
        return ApplicationProtocol::UNKNOWN;
    }
    
    if(transProtocol == TranportProtocol::TCP)
    {
        switch(dstPort)
        {
            case 20: return ApplicationProtocol::FTP;
            case 21: return ApplicationProtocol::FTP;
            case 22: return ApplicationProtocol::SSH;
            case 25: return ApplicationProtocol::SMTP;
            case 53: return ApplicationProtocol::DNS;
            case 80: return ApplicationProtocol::HTTP;
            case 443: return ApplicationProtocol::HTTPS;
            case 465: return ApplicationProtocol::SMTP;
            case 587: return ApplicationProtocol::SMTP;
            default: return ApplicationProtocol::UNKNOWN;
        }
    } 
    else if(transProtocol == TransportProtocol::UDP)
    {
        switch(dstPort)
        {
            case 53: return ApplicationProtocol::DNS; 
            case 123: return ApplicationProtocol::NTP;
            case 443: return ApplicationProtocol::QUIC;
            default: return ApplicationProtocol::UNKNOWN;
        }
    }

    return ApplicationProtocol::UNKWONW;
}
