#ifndef PROTOCOL_STATS_H
#define PROTOCOL_STATS_H

#include "Alerts.h"
#include "Packet.h"

#include <chrono>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <unordered_map>

struct BandwidthPoint
{
    double timestamp;
    double bytesPerSecond;
};

struct IPStats
{
    uint32_t bytesReceived = 0;
    uint32_t bytesSent = 0;

    uint32_t packetsReceived = 0;
    uint32_t packetsSent = 0;

    std::set<uint16_t> portsContacted;
    std::chrono::steady_clock::time_point timestamp;
};

struct protocolStats
{
    uint32_t bytes = 0;
    uint32_t packets = 0;
};

struct trafficStats
{
    uint32_t totalBytes = 0;
    uint32_t totalPackets = 0;
};

struct StatsSnapshot
{
    std::vector<std::vector<std::string>> applicationRow;
    std::vector<std::vector<std::string>> transportRow;
    std::vector<std::vector<std::string>> ipRows;
    std::vector<std::vector<std::string>> pairRows;
    std::vector<std::vector<std::string>> packetRows;
        
    std::vector<Alert> alerts;

    uint32_t currentBytes = 0;
    uint32_t totalBytes = 0;
    uint32_t totalPackets = 0;

    std::vector<BandwidthPoint> bandwidthHistory;
    double bandwidth = 0;
    double maxBandwidth = 0;
};


class Stats
{
    std::mutex mtx;
    
    uint32_t lastByte= 0;
    std::chrono::steady_clock::time_point lastTick;
        
    std::unordered_map<TransportProtocol, protocolStats> transportMap;
    std::unordered_map<ApplicationProtocol, protocolStats> applicationMap;
    std::unordered_map<std::string, IPStats> ipMap;

    std::map<std::pair<std::string, std::string>, protocolStats> pairs;

    std::deque<Packet> packets;
    
    StatsSnapshot snapshot;

private:
    void verifyAnomaly(const std::string& ipSrc);

public:
    Stats();

    void push(const Packet& packet)
    {
        std::scoped_lock<std::mutex> lck(mtx);
        packets.push_back(packet);
    }

    void addPacket(const Packet& packet);

    StatsSnapshot getSnapshot() 
    { 
        std::scoped_lock<std::mutex> lck(mtx);
        return snapshot; 
    }

    void updateAppStats();
    void updateBandwidth();
    double smooth_bandwidth = 0.0;


    void updateIpStats();
    void updatePackets();
    void updatePairs();
    void updateTransportStats();

    void exportCsv();
    void exportJson();
};

#endif
