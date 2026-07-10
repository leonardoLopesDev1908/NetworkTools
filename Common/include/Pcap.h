#ifndef PCAP_H
#define PCAP_H

#ifdef _WIN32
#include <pcap.h>
#else
#include <pcap/pcap.h> 
#endif

#endif