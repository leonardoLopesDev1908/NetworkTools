#include "View.h"

ftxui::Element render(const StatsSnapshot &data, const std::string &interface, 
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
                    renderbandwidth(data) | border | flex});

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

static ftxui::Element renderHeader(const StatsSnapshot &data, 
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
		   }) |
		   border;
}

static ftxui::Element renderStats(const StatsSnapshot &data)
{
    return vbox({text("=== Traffic summary ===") | bold, 
                text("Total packets: " + std::to_string(data.total_p)),
			    text(std::format("Total bytes  : {:.2f} MB", 
                data.total_b / (1024.0 * 1024.0)))
            }) |
		    flex;
}

static ftxui::Element renderTransport(const StatsSnapshot &data)
{
    Table table(data.transport_rows);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);

	return vbox({text("=== Transport protocols === ") | bold, table.Render()}) | flex;
}

static ftxui::Element renderApplication(const StatsSnapshot &data)
{
    Table table(data.app_rows);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);

	return vbox({text("=== Application protocols ===") | bold, table.Render()}) | flex;
}

static ftxui::Element renderIp(const StatsSnapshot &data)
{
    Table table(data.rows);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);

	return vbox({text("=== Top IP addresses ===") | bold,

				 table.Render()}) |
		   flex;
}

static ftxui::Element renderBandwidth(const StatsSnapshot &data)
{
    GraphFunction fn = [data](int width, int height) {
		std::vector<int> output(width, 0);

		if (data.bandwidth_history.size() < 2) {
			return output;
		}

		const size_t n = data.bandwidth_history.size();
		const size_t start = n > 50 ? n - 50 : 0;

		double max_bw = 1.0;
		for (size_t i = start; i < n; ++i) {
			max_bw = std::max(max_bw, data.bandwidth_history[i].bytes_per_sec);
		}

		for (int x = 0; x < width; ++x) {
			const double t = static_cast<double>(x) / static_cast<double>(width - 1);

			const double idx_f = static_cast<double>(start) + t * static_cast<double>(n - start - 1);
			const size_t i0 = static_cast<size_t>(idx_f);
			const size_t i1 = std::min(i0 + 1, n - 1);

			const double frac = idx_f - static_cast<double>(i0);
			const double bw = data.bandwidth_history[i0].bytes_per_sec * (1.0 - frac) +
							  data.bandwidth_history[i1].bytes_per_sec * frac;

			const double v = bw / max_bw;
			output[x] = static_cast<int>(v * static_cast<double>(height - 1));
		}

		return output;
	};
	return vbox({
		text(std::format(" Bandwidth: {:.2f} KB / max: {:.2f} KB", data.bandwidth, data.max_bandwidth)) | bold,
		graph(fn) | size(HEIGHT, EQUAL, 20) | size(WIDTH, EQUAL, 60) | border | color(Color::Green),
	});
}

static ftxui::Element renderPackets(const StatsSnapshot &data)
{
    Table table(data.packets_rows);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);
	return vbox({text("=== Packets ===") | bold,

				 table.Render() | flex}) |
		   flex;
}


static ftxui::Element renderFooter(bool captureFinished, std::chrono::seconds timer)
{
    return Element({capture_finished
						? text("Capture finished (" + std::format("{}", timer) + "). Press 'q' or Esc to exit.") |
							  bold | color(Color::Yellow) | center
						: text("time: " + std::format("{}", timer) + ". Press 'q' or Esc to exit.") | center |
							  size(HEIGHT, EQUAL, 1)});
}

static ftxui::Element View::renderPairs(const StatsSnapshot &data) {
	Table table(data.pairs_rows);
	table.SelectAll().Border(LIGHT);

	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).SeparatorVertical(LIGHT);
	table.SelectRow(0).Decorate(bold);
	table.SelectRow(0).Border(DOUBLE);

	return vbox({text("=== Top communication pairs ===") | bold, table.Render()}) | flex;
}
