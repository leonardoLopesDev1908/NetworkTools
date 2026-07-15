#ifndef ANALYZER_APP_H
#define ANALYZER_APP_H

#include "Capture.h"
#include "Stats.h"
#include "View.h"

#include <iostream>
#include <vector>

class AnalyzerApp
{
public:

	void start(const std::string& intf, int limitPackets, 
			std::vector<std::string>& filters);
	void printHelp();

private:

	Capture capture;
	Stats stats;
    View view;
};

#endif