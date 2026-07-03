#ifndef PROTOCOL_STATS_H
#define PROTOCOL_STATS_H

#include "Packet.h"

#include <chrono>
#include <map>
#include <queue>
#include <unordered_map>

struct trafficStats
{
    uint32_t totalBytes = 0;
    uint32_t totalPackets = 0;
};

struct protocolStats 
{
    uint32_t bytes = 0;
    uint32_t packets = 0;
};

struct IPStats 
{
    uint32_t bytesReceived = 0;
    uint32_t bytesSent = 0;

    uint32_t packetsReceived = 0;
    uint32_t packetsSent = 0;
};

struct StatsSnapshot
{
    std::vector<std::vector<std::string>> applicationRow;
    std::vector<std::vector<std::string>> transportRow;
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<std::string>> pairRows;
    std::vector<std::vector<std::string>> packetRows;


};


class Stats
{
    std::chrono::steady_clock::time_point clock;
        
    std::unordered_map<TransportProtocol, protocolStats> transportMap;
    std::unordered_map<ApplicationProtocol, protocolStats> applicationMap;
    std::unordered_map<std::string, IPStats> ipMap;

    std::map<std::pair<std::string, std::string>, protocolMap>> pairs;

    std::deque<Packet> packets;
    
    StatsSnapshot snapshot;
public:
    void addPacket(Packet& packet);

};


#endif
