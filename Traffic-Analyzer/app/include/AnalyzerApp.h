#ifndef ANALYZER_APP_H
#define ANALYZER_APP_H

#include "Capture.h"
#include "Stats.h"
#include "View.h"

#include <iostream>
#include <vector>

class AnalyzerApp
{
	Capture capture;
	Stats stats;
    View view;
    std::string output;

public:

	void start(const std::string& intf, int limitPackets, 
			std::vector<std::string>& filters, const std::string& output);
	void printHelp();
};


#endif
