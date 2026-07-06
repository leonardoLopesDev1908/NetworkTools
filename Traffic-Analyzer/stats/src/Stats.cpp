#include "Stats.h"

Stats::Stats() : lastTick(std::chrono::steady_clock::now()) {}

void Stats::addPacket(const Packet& packet)
{	
    std::scoped_lock<std::mutex> lck(mtx);

	transportMap[packet.transProtocol].bytes += packet.totalLen;
	transportMap[packet.transProtocol].packets++;

	applicationMap[packet.appProtocol].bytes += packet.totalLen;
	applicationMap[packet.appProtocol].packets++;

	ipMap[packet.source].bytesSent += packet.totalLen;
    ipMap[packet.source].packetsSent++;

	ipMap[packet.destiny].bytesReceived += packet.totalLen;
    ipMap[packet.destiny].packetsReceived++;
	
	snapshot.totalBytes += packet.totalLen;
    snapshot.currentBytes += packet.totalLen; 
    snapshot.totalPackets++;

	pairs[{packet.source, packet.destiny}].bytes += packet.totalLen;
	pairs[{packet.source, packet.destiny}].packets++;
}

void Stats::updateAppStats() 
{ 
    std::scoped_lock<std::mutex> lck(mtx);
    snapshot.applicationRow.clear();
    snapshot.applicationRow.push_back({"Proto", "Packets", "Bytes (MB)", "%"});

    std::string buffer;

    for (auto& [app, stats] : applicationMap)
    {
        buffer = std::to_string(((double)stats.bytes / snapshot.totalBytes) * 100);
        
        snapshot.applicationRow.push_back({
            appToStr(app), std::to_string(stats.packets), 
            std::to_string(stats.bytes), buffer
        });

        buffer.clear();
    }
}

void Stats::updateBandwidth() 
{ 
    std::scoped_lock<std::mutex> lck(mtx);

    auto now = std::chrono::steady_clock::now();
    auto timestamp = now - lastTick;

    BandwidthPoint bd{};
    bd.timestamp = timestamp.count();
    bd.bytesPerSecond = snapshot.currentBytes / timestamp.count();

    snapshot.bandwidthHistory.emplace_back(bd);
    snapshot.bandwidth = ((double)snapshot.currentBytes * 8) / timestamp.count();

    if (snapshot.bandwidth > snapshot.maxBandwidth)
        snapshot.maxBandwidth = snapshot.bandwidth;

    lastTick = std::chrono::steady_clock::now();
    snapshot.currentBytes = 0;
}

void Stats::updateIpStats()
{
    std::scoped_lock<std::mutex> lck(mtx);

    snapshot.ipRows.clear();
    snapshot.ipRows.push_back({"IP Address", "Packets sent", "Packets received"});

    for (auto& [ip, stats] : ipMap)
    {
        snapshot.ipRows.push_back(
            {ip, std::to_string(stats.packetsSent), std::to_string(stats.packetsReceived)
        });
    }
}

void Stats::updatePackets()
{ 
    std::scoped_lock<std::mutex> lck(mtx);
    snapshot.packetRows.clear();
    snapshot.packetRows.push_back(
        {"IP Version", "Trasnport protocol", "Source", "Destination", "App protocol"}
    );

    for (int i = 0; i < 50; i++)
    {
        snapshot.packetRows.push_back({
            std::to_string(packets[i].ipVersion), transportToStr(packets[i].transProtocol), 
            packets[i].source,packets[i].destiny, appToStr(packets[i].appProtocol)
        });
    }
}

void Stats::updatePairs()
{ 
    std::scoped_lock<std::mutex> lck(mtx);

    snapshot.pairRows.clear();
    snapshot.pairRows.push_back({"Source", "Destination", "bytes recv", "%"});

    std::string buffer;
    for (auto& [pair, stats] : pairs)
    {
        buffer = std::to_string(((double)stats.packets / snapshot.totalBytes) * 100);
        
        snapshot.pairRows.push_back({
            pair.first, pair.second, 
            std::to_string(stats.bytes), buffer
        });

        buffer.clear();
    }
}

void Stats::updateTransportStats()
{
    std::scoped_lock<std::mutex> lck(mtx);
    snapshot.transportRow.clear();
    snapshot.transportRow.push_back({"Proto", "Packets", "Bytes (MB)", "%"});
    std::string buffer;

    int i = 0;
    for (auto& [trp, stats] : transportMap)
    {
        buffer = std::to_string((stats.packets / snapshot.totalBytes) * 100);
        
        snapshot.transportRow.push_back({
            transportToStr(trp), std::to_string(stats.packets),
            std::to_string(stats.bytes), buffer
        });
        
        buffer.clear();

        if (++i == 50)
            break;
    }
}

static const char* transportToStr(TransportProtocol transport)
{
    switch (transport)
    {
    case TransportProtocol::TCP:
        return "TCP";
    case TransportProtocol::UDP:
        return "UDP";
    case TransportProtocol::ICMP:
        return "ICMP";
    case TransportProtocol::ICMP6:
        return "ICMP6";
    case TransportProtocol::IGMP:
        return "IGMP";
    default:
        return "UNKNOWN";
    }
}

static const char* appToStr(ApplicationProtocol app)
{
    switch (app)
    {
    case ApplicationProtocol::HTTP:
        return "HTTP";
    case ApplicationProtocol::HTTPS:
        return "HTTPS";
    case ApplicationProtocol::DNS:
        return "DNS";
    case ApplicationProtocol::FTP:
        return "FTP";
    case ApplicationProtocol::SSH:
        return "SSH";
    case ApplicationProtocol::SMTP:
        return "SMTP";
    case ApplicationProtocol::QUIC:
        return "QUIC";
    case ApplicationProtocol::NTP:
        return "NTP";
    default:
        return "UNKNOWN";
    }
}
