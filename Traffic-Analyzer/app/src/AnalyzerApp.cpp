#include "AnalyzerApp.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

void AnalyzerApp::start(const std::string& intf, int limitPackets, 
					std::vector<std::string>& filters)
{
    std::string filterExp = "";
	if (!filters.empty())
	{
        for (auto& s : filters)
        {
            filterExp += s + " ";
        }
	}

	capture.config(intf, limitPackets, &stats, filterExp);

    std::atomic<bool> captureFinished = false;
    std::atomic<bool> tuiRunning = true;

	capture.start();

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::atomic<std::chrono::seconds> timer;
    ftxui::Element currentFrame = ftxui::text("Starting capture...");

    auto screen = ftxui::ScreenInteractive::Fullscreen();

    std::mutex mtx;
    
    auto component = ftxui::Renderer(
        [&]
        {
            std::lock_guard<std::mutex> lock(mtx);
            return currentFrame;
        });

    component |= ftxui::CatchEvent(
        [&](ftxui::Event e)
        {
            if (e == ftxui::Event::Character('q') || e == ftxui::Event::Escape)
            {
                tuiRunning = false;
                screen.Exit();
                return true;
            }
            return true;
        });

	std::thread appThread = std::thread([&]
        {
            while (capture.isRunning() || tuiRunning)
            {
                auto now = std::chrono::steady_clock::now();
                timer.store(std::chrono::duration_cast<std::chrono::seconds>(now - begin));

                if (!tuiRunning)
                    captureFinished = true;

                stats.updateAppStats();
                stats.updateBandwidth();
                stats.updateIpStats();
                stats.updatePackets();
                stats.updatePairs();
                stats.updateTransportStats();

                ftxui::Element newFrame = view.render(stats.getSnaphot(), intf, filterExp, 
                                            captureFinished, timer.load());

                {
                    std::scoped_lock<std::mutex> lck(mtx);
                    currentFrame = newFrame;
                }
                if (tuiRunning)
                {
                    screen.PostEvent(ftxui::Event::Custom);
                }
            }
        });

    screen.Loop(component);

    captureFinished = false;

    if (appThread.joinable())
        appThread.join();
}

void AnalyzerApp::printHelp()
{
    std::cout << R"(
		This part of the project is almost entirely inspired 
		by the Traffic Analyzer at the Cybersecurity-Projects repository 
		developed by @deniskhud. It is available at: 
		https://github.com/CarterPerez-dev/Cybersecurity-Projects/tree/main/PROJECTS/beginner/network-traffic-analyzer/cpp
	)";
}