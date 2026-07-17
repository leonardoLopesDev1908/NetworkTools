#ifndef CAPTURE_H
#define CAPTURE_H

#include "IP.h"
#include "Packet.h"
#include "Pcap.h"
#include "Stats.h"

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

class Capture
{
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    std::string device;

    int packetsLimit{};

    int linkHdrType{};
    unsigned int linkLen{};

    //std::unique_ptr<type, decltype(&close_func)> name{nullptr, &close_func};
    std::unique_ptr<pcap_t, decltype(&pcap_close)> handle {nullptr, &pcap_close};
    
    struct bpf_program fp{};
    bpf_u_int32 netmask{};

    uint16_t offset = 0;
    std::function<uint16_t(const uint8_t*)> getEtherType;
    pcap_if_t* interfaces = nullptr;
    pcap_if_t* devicePtr = nullptr;

    std::string filterExp;
    std::atomic<bool> running{false};
    std::thread thread;
    Stats* stats = nullptr;

private:

    void dataLink(int type);

public:
    
    ~Capture();

    void initialize();
    void config(const std::string& device, int limit, Stats* stats, const std::string& filterExp);
    void start();
    void stop();

    static void callback(uint8_t* user, const struct pcap_pkthdr *pkthdr, 
              const uint8_t* packetd_ptr);

    void treatPacket(const struct pcap_pkthdr* pkthdr, const uint8_t* packet);
    
    bool isRunning() { return running; }
    void setRunning(bool flag) { running = flag; }
    std::string getDevice() const { return device; }
};


#endif
