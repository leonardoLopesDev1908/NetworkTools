#ifndef CAPTURE_H
#define CAPTURE_H

#include "IP.h"
#include "Packet.h"
#include "Platform.h"
#include "Stats.h"

#include <atomic>
#include <memory>
#include <pcap/pcap.h>
#include <thread>

class Capture
{
    char errBuf[PCAP_ERRBUF_SIZE]{};
    std::string device;

    int packetsLimit{};
    //std::unique_ptr<type, decltype(&close_func)> name{nullptr, &close_func};
    std::unique_ptr<pcap_t, decltype(&pcap_close)> handle {nullptr, &pcap_close};

    struct bpf_program fp{};
    std::string filterExp;

    pcap_if_t *interfaces = nullptr;

    std::atomic<bool> running{false};
    std::thread;
    Stats* stats = nullptr;

private:

    void stop();

public:
            
    void initialize();
    void start();
    void config(const std::string& device, int limit, Stats* stats,
            const std::string& filter);

    void callback(uint8_t* user, const struct pcap_pkthdr *pkthdr, 
              const uint8_t* packetd_ptr);
    
    bool isRunning() { return running; }
    void setRunning(bool flag) { running = flag; }
   
    void printInterfaces();
};


#endif
