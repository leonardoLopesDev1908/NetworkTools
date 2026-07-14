#include "AnalyzerApp.h"
#include "Initialization.h"
#include "Platform.h"
#include "ProxyApp.h"
#include <boost/program_options.hpp>
#include <expected>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    platformInit();

    po::variables_map vm;
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Display this help message and exit")
        ("tools", po::value<std::string>()->default_value("analyzer"),
            "Tool mode to execute: 'proxy' or 'analyzer' ")

        ("interfaces, interfaces", "Display all possible interfaces")
        ("interface,i", po::value<std::string>()->default_value("wlan0"))

        ("count,c", po::value<int>()->default_value(0),
            "Number of packets to capture (0 = unlimited)")(
            "time, t", po::value<int>()->default_value(INT_MAX), "Working time (in seconds)")

        ("filter,f", po::value<std::vector<std::string>>()->composing(),
            "Traffic filter (can be used multiple times)\n"
            "  proto:<name>   tcp | udp | icmp | dns\n"
            "  src:<ip>       Source IP address\n"
            "  dst:<ip>       Destination IP address\n"
            "  port:<number>  Source or destination port");

    po::positional_options_description p;
    p.add("tools", 1);

    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);
    
    std::string tool;
    
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    if (vm.count("tools"))
    {
        tool = vm["tools"].as<std::string>();          
    }

    if (tool == "proxy")
    {
        ProxyApp proxy;
	    proxy.start();
    }
    else
    {
        std::string intf{};
        
        if (vm.count("interface"))
            intf = vm["interface"].as<std::string>();
        
        int limitPackets{};
        if (vm.count("count"))
            limitPackets = vm["count"].as<int>();

        std::vector<std::string> filters{};
        if (vm.count("filters"))
            filters = vm["filters"].as <std::vector<std::string>>();

        AnalyzerApp trafficAnalyzer;
        trafficAnalyzer.start(intf, limitPackets, filters);
    }

    platformCleanup();

	return 0;
}
