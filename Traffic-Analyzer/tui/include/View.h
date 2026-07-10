#ifndef VIEW_H
#define VIEW_H

#include "Stats.h"

#include <ftxui/dom/elements.hpp>

class View
{
public:
    ftxui::Element render(const StatsSnapshot &data, const std::string &interface, const std::string &filter,
						  bool capture_finished, std::chrono::seconds timer);

private:
	static ftxui::Element renderHeader(const StatsSnapshot &data, const std::string &interface,
										const std::string &filter);
	static ftxui::Element renderStats(const StatsSnapshot &data);

	static ftxui::Element renderTransport(const StatsSnapshot &data);
	static ftxui::Element renderApplication(const StatsSnapshot &data);
	static ftxui::Element renderIp(const StatsSnapshot &data);
	static ftxui::Element renderPairs(const StatsSnapshot &data);
	static ftxui::Element renderBandwidth(const StatsSnapshot &data);
	static ftxui::Element renderPackets(const StatsSnapshot &data);

	static ftxui::Element renderFooter(bool capture_finished, std::chrono::seconds timer);

};


#endif
