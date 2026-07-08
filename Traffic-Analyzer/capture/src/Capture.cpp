#include "Capture.h"

Capture::~Capture() { stop(); }

void Capture::stop() {}

void Capture::initialize() 
{
    if (pcap_findalldevs(&interfaces, errBuf) == -1)
	    throw std::runtime_error("Error: pcap_findalldevs failed");
}

void Capture::start() 
{


         
    handle.reset(pcap_open_live(device.c_str(), BUFSIZ, 1000, errBuf));
    
    if(handle == nullptr)
        throw std::runtime_error("Error: " + device + " failed: " + errBuf);


}

void Capture::config(const std::string& device, int limit, Stats* stats,          const std::string& filterExp);
{
    this->device = device;
    this->stats = stats;
    this->filterExp = filterExp;

    packetsLimit = limit;
}

void Capture::printInterfaces()
{

}
