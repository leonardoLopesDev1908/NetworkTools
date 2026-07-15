#include "Capture.h"

#include <print>
#include <fstream>

Capture::~Capture(){ stop(); }

static uint16_t getEtherByFamily(const uint32_t type)
{ 
    switch (type)
    {
    case AF_INET:
        return ETHERTYPE_IP;
    case AF_INET6:
        return ETHERTYPE_IPV6;
    default:
        return 0;
    }
}

void Capture::stop() 
{
    running = false;
    if (thread.joinable())
        thread.join();
}

void Capture::callback(uint8_t* user, const struct pcap_pkthdr* pkthdr, const uint8_t* packetd_ptr)
{
    auto* self= reinterpret_cast<Capture*>(user);
    
    if (!self->isRunning())
    {
        return;
    }

    self->treatPacket(pkthdr, packetd_ptr);
}

void Capture::treatPacket(const struct pcap_pkthdr* pkthdr, const uint8_t* packet)
{
    uint16_t etherType = getEtherType(packet);

    if (etherType == ETHERTYPE_IP)
    {
        IPv4 ip(packet + offset);

        Packet pkt(v4, ip.getProtocol(), ip.getSource(), ip.getDestiny(), 
            ip.getSourcePort(), ip.getDestinyPort(), pkthdr->len, 
            ip.getPayloadLen(), ip.getPayloadPtr());
        
        stats->push(pkt);
        stats->addPacket(pkt);
    }
    else if (etherType == ETHERTYPE_IPV6)
    {
        IPv6 ip (packet + offset);

        Packet pkt(v6, ip.getProtocol(), ip.getSource(), ip.getDestiny(), 
            ip.getSourcePort(), ip.getDestinyPort(), pkthdr->len, 
            ip.getPayloadLen(), ip.getPayloadPtr());

        stats->push(pkt);
        stats->addPacket(pkt);
    }
}

void Capture::dataLink(int type)
{ 
    switch (type)
    {
    case DLT_EN10MB:
        offset = 14;
        getEtherType = [](const uint8_t* p)
        { 
            const auto* eth = reinterpret_cast<const ether_header*>(p);
            return ntohs(eth->ether_type);
        };
        break;
    case DLT_NULL:
        offset = 4;
        getEtherType = [](const uint8_t* p)
        {
            const uint32_t ip = *reinterpret_cast<const uint32_t*>(p);
            return getEtherByFamily(ip);
        };
        break;
    case DLT_LOOP:
        offset = 4;
        getEtherType = [](const uint8_t* p)
        { 
            const uint32_t ip = ntohs(*reinterpret_cast<const uint32_t*>(p));
            return getEtherByFamily(ip); 
        };
        break;
    case DLT_LINUX_SLL:
        offset = 16;
        getEtherType = [](const uint8_t* p)
        { 
            return ntohs(*reinterpret_cast<const uint16_t*>(p + 14)); 
        };
        break;
    case DLT_LINUX_SLL2:
        offset = 20;
        getEtherType = [](const uint8_t* p)
        { 
            return ntohs(*reinterpret_cast<const uint16_t*>(p + 18)); 
        };
        break;
    default:
        throw std::runtime_error("Data link not supported");
    }
}

void Capture::initialize()
{
    if (pcap_findalldevs(&interfaces, errBuf) == -1)
    {
        throw std::runtime_error("Error: pcap_findalldevs has been failed");
    }
}

void Capture::start() 
{
    //This is the way of chosing an interface in windows platforms
    //as windows network interfaces have not the same naming conventions as linux
#ifdef _WIN32
    int i = 1;
    for (devicePtr = interfaces; devicePtr != nullptr; devicePtr = devicePtr->next)
    {
        std::print("%d: %s\n", i++, devicePtr->name);
        std::print("%s\n\n", devicePtr->description);
    }

    if (i == 0)
        throw std::runtime_error("No interface was found");

    int input{};
    std::print("Number of the chosen interface: ");
    std::cin >> input;
    
    if (input > i || input < 1)
    {
        pcap_freealldevs(interfaces);
        throw std::runtime_error("Invalid option");
    }

    for (devicePtr = interfaces, i = 0; i < input - 1; i++, devicePtr = devicePtr->next)
        ;

    device = devicePtr->name;
    handle.reset(pcap_open_live(devicePtr->name, BUFSIZ, 1, 100, errBuf));

#else
    handle.reset(pcap_open_live(device.c_str(), BUFSIZ, 1, 100, errBuf));
#endif

    if (handle == nullptr)
    {
        std::print("Error: %s failed: %s\n", device.c_str(), errBuf);
        return;
    }
        
    dataLink(pcap_datalink(handle.get()));
        
    running = true;

    if (!filterExp.empty())
    {
        if (pcap_compile(handle.get(), &fp, filterExp.c_str(), 0, netmask) == PCAP_ERROR)
            std::print("Error: pcap_compile\n");

        if (pcap_setfilter(handle.get() , &fp) == 0)
            std::print("Error: pcap_setfilter() %s", pcap_geterr(handle.get()));
    }

    thread = std::thread([&](){
        if (pcap_loop(handle.get(), packetsLimit, callback, reinterpret_cast<uint8_t*>(this)))
        {
            std::print("Error: pcap_loop() faile\n");
            exit(1);
        }
    });
}

void Capture::config(const std::string& device, int limit, Stats* stats, const std::string& filterExp)
{
    this->device = device;
    this->stats = stats;
    this->filterExp = filterExp;

    packetsLimit = limit;
}

void Capture::printInterfaces()
{

}
