#include "Stats.h"

#include <filesystem>
#include <fstream>

Stats::Stats() : lastTick(std::chrono::steady_clock::now()) {}

const char* transportToStr(TransportProtocol transport)
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

const char* appToStr(ApplicationProtocol app)
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

void Stats::addPacket(const Packet& packet)
{	
    std::scoped_lock<std::mutex> lck(mtx);

	transportMap[packet.transProtocol].bytes += packet.totalLen;
	transportMap[packet.transProtocol].packets++;

	applicationMap[packet.appProtocol].bytes += packet.totalLen;
	applicationMap[packet.appProtocol].packets++;

	ipMap[packet.source].bytesSent += packet.totalLen;
    ipMap[packet.source].packetsSent++;

    auto now = std::chrono::steady_clock::now();
    if (now - ipMap[packet.source].timestamp > std::chrono::seconds(10))
    {
        ipMap[packet.source].portsContacted.clear();
        ipMap[packet.source].timestamp = std::chrono::steady_clock::now();
    }
    else
        ipMap[packet.source].portsContacted.emplace(packet.dstPort);

    ipMap[packet.destiny].bytesReceived += packet.totalLen;
    ipMap[packet.destiny].packetsReceived++;

	snapshot.totalBytes += packet.totalLen;
    snapshot.currentBytes += packet.totalLen; 
    snapshot.totalPackets++;

	pairs[{packet.source, packet.destiny}].bytes += packet.totalLen;
	pairs[{packet.source, packet.destiny}].packets++;

    verifyAnomaly(packet.source);
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
    std::lock_guard<std::mutex> lock(mtx);
    using namespace std::chrono;

    auto now = steady_clock::now();
    double ts = duration_cast<duration<double>>(now.time_since_epoch()).count();
    double elapsed = duration_cast<duration<double>>(now - lastTick).count();

    if (elapsed >= 1.0)
    {

        uint32_t delta_bytes = snapshot.totalBytes - lastByte;

        snapshot.bandwidth = delta_bytes / elapsed; 

        lastByte = snapshot.totalBytes;
        lastTick = now;
        const double alpha = 0.2;
        smooth_bandwidth = alpha * snapshot.bandwidth + (1.0 - alpha) * smooth_bandwidth;

        snapshot.bandwidthHistory.push_back({ts, smooth_bandwidth});
    }
    snapshot.maxBandwidth= std::max(snapshot.maxBandwidth, snapshot.bandwidth);
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
        {"IP Version", "Transport protocol", "Source", "Destination", "App protocol"}
    );

    for (auto& p : packets)
    {
        snapshot.packetRows.push_back({
            std::to_string(p.ipVersion), transportToStr(p.transProtocol), 
            p.source,p.destiny, appToStr(p.appProtocol)
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

void Stats::verifyAnomaly(const std::string& ipSrc)
{ 
    if (ipMap[ipSrc].portsContacted.size() >= 15)
    {
        snapshot.alerts.emplace_back(AnomalyType::PortScan, Risk::Medium, ipSrc, "Port scan");
    }
}

void Stats::exportCsv()
{
    std::filesystem::path path = std::filesystem::current_path();
    std::filesystem::path outputFile = path / "capture_out.csv";

    std::ofstream csvFile(outputFile);

    if (csvFile.is_open())
    {
        csvFile << "Name,Packet,Bytes\n";

        csvFile << "Total," << snapshot.totalPackets << "," << snapshot.totalBytes << "\n";

        for (auto& [t, s] : transportMap)
            csvFile << transportToStr(t) << "," << s.packets << "," << s.bytes << "\n";

        for (auto& [app, s] : applicationMap)
            csvFile << appToStr(app) << "," << s.packets << "," << s.bytes << "\n";

        for (auto& [ip, s] : ipMap)
            csvFile << ip << "," << s.packetsSent << "," << s.bytesSent << "\n";

        printf("report created\n");
    }
    else
    {
        throw std::runtime_error("Unable to create the report file");
    }
}

