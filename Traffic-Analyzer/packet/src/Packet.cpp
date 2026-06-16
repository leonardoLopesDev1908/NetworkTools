#include "Packet.h"

ApplicationProtocol Packet::getApplicationProtocol()
{
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
        }
    } else if(transProtocol == TransportProtocol::UDP)
    {
        switch(dstPort)
        {
            
        }
    }

}
