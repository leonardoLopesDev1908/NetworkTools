#include "View.h"
#include "ftxui/dom/table.hpp"

using namespace ftxui;

ftxui::Element View::render(const StatsSnapshot &data, const std::string &interface, 
	const std::string &filter, bool captureFinished, std::chrono::seconds timer)
{
    auto header = renderHeader(data, interface, filter);

	auto transportSection = hbox({
								 renderTransport(data) | flex,
								 separator(),
								 renderApplication(data) | flex,
								 separator(),
								 renderPairs(data) | flex,
							 }) |
							 border;

	auto ipSection = hbox({renderIp(data) 
                    | border | size(HEIGHT, LESS_THAN, 10) 
                    | frame | vscroll_indicator, 
                    renderBandwidth(data) | border | flex});

	auto leftPanel = vbox({
						  transportSection,
						  separator(),
						  ipSection,
					  }) |
					  flex_grow;

	auto rightPanel = renderPackets(data) 
                    | border | size(WIDTH, EQUAL, 100) 
                    | frame | vscroll_indicator;

	auto body = hbox({
					leftPanel,
					separator(),
					rightPanel,
				}) |
				flex;

	auto footer = renderFooter(captureFinished, timer);

	return vbox({
		header,
		separator(),
		body,
		separator(),
		footer,
	});
}

ftxui::Element View::renderHeader(const StatsSnapshot &data, 
        const std::string &interface, const std::string& filter)
{
	return hbox({
			   vbox({
				   text("Network Traffic Analyzer") | bold,
				   text("Interface: " + interface),
				   text("Filter: " + filter),
			   }) | flex,
			   separator(),
			   renderStats(data) | flex,
		   }) | border;
}

ftxui::Element View::renderStats(const StatsSnapshot &data)
{
    return vbox({text("=== Traffic summary ===") | bold, 
                text("Total packets: " + std::to_string(data.totalPackets)),
			    text(std::format("Total bytes  : {:.2f} MB", 
                data.totalBytes / (1024.0 * 1024.0)))
            }) | flex;
}

ftxui::Element View::renderTransport(const StatsSnapshot &data)
{
    Table table(data.transportRow);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);

	return vbox({text("=== Transport protocols === ") | bold, table.Render()}) | flex;
}

ftxui::Element View::renderApplication(const StatsSnapshot &data)
{
    Table table(data.applicationRow);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);

	return vbox({text("=== Application protocols ===") | bold, table.Render()}) | flex;
}

ftxui::Element View::renderIp(const StatsSnapshot &data)
{
    Table table(data.ipRows);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);

	return vbox({text("=== Top IP addresses ===") 
		| bold, table.Render()}) | flex;
}

ftxui::Element View::renderBandwidth(const StatsSnapshot &data)
{
    GraphFunction fn = [data](int width, int height) {
		std::vector<int> output(width, 0);

		if (data.bandwidthHistory.size() < 2) {
			return output;
		}

		const size_t n = data.bandwidthHistory.size();
		const size_t start = n > 50 ? n - 50 : 0;

		double max_bw = 1.0;
		for (size_t i = start; i < n; ++i) {
			max_bw = std::max(max_bw, data.bandwidthHistory[i].bytesPerSecond);
		}

		for (int x = 0; x < width; ++x) {
			const double t = static_cast<double>(x) / static_cast<double>(width - 1);

			const double idx_f = static_cast<double>(start) + t * static_cast<double>(n - start - 1);
			const size_t i0 = static_cast<size_t>(idx_f);
			const size_t i1 = std::min(i0 + 1, n - 1);

			const double frac = idx_f - static_cast<double>(i0);
			const double bw = data.bandwidthHistory[i0].bytesPerSecond * (1.0 - frac) +
							  data.bandwidthHistory[i1].bytesPerSecond * frac;

			const double v = bw / max_bw;
			output[x] = static_cast<int>(v * static_cast<double>(height - 1));
		}

		return output;
	};
	return vbox({
		text(std::format(" Bandwidth: {:.2f} KB / max: {:.2f} KB", data.bandwidth, data.maxBandwidth)) | bold,
		graph(fn) | size(HEIGHT, EQUAL, 20) | size(WIDTH, EQUAL, 60) | border | color(Color::Green),
	});
}

ftxui::Element View::renderPackets(const StatsSnapshot &data)
{
    Table table(data.packetRows);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);
	return vbox({text("=== Packets ===") 
		| bold, table.Render() | flex}) | flex;
}


ftxui::Element View::renderFooter(bool captureFinished, std::chrono::seconds timer)
{
    return Element({captureFinished
						? text("Capture finished (" + std::format("{}", timer) + "). Press 'q' or Esc to exit.") |
							  bold | color(Color::Yellow) | center
						: text("time: " + std::format("{}", timer) + ". Press 'q' or Esc to exit.") | center |
							  size(HEIGHT, EQUAL, 1)});
}

ftxui::Element View::renderPairs(const StatsSnapshot &data) {
	Table table(data.pairRows);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);

	return vbox({text("=== Top communication pairs ===") | bold, table.Render()}) | flex;
}
