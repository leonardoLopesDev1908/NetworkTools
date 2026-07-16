#include "AnalyzerApp.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

void AnalyzerApp::start(const std::string& intf, int limitPackets, 
					std::vector<std::string>& filters, const std::string& output)
{
    std::string filterExp = "";
	if (!filters.empty())
	{
        for (auto& s : filters)
        {
            filterExp += s + " ";
        }
	}

    std::atomic<bool> captureFinished = false;
    std::atomic<bool> tuiRunning = true;

	capture.config(intf, limitPackets, &stats, filterExp);
    capture.initialize();
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
            return false;
        });
        
	std::thread appThread = std::thread([&]
        {
            const std::chrono::milliseconds frameDuration(33);

            while (capture.isRunning() || tuiRunning)
            {
                auto frameStart = std::chrono::steady_clock::now();

                if (!tuiRunning)
                    captureFinished = true;

                stats.updateAppStats();
                stats.updateBandwidth();
                stats.updateIpStats();
                stats.updatePackets();
                stats.updatePairs();
                stats.updateTransportStats();

                ftxui::Element newFrame = view.render(stats.getSnapshot(), intf, filterExp, 
                                           captureFinished, timer.load());

                {
                    std::scoped_lock<std::mutex> lck(mtx);
                    currentFrame = newFrame;
                }
                
                if (tuiRunning)
                {
                   screen.PostEvent(ftxui::Event::Custom);
                }

                auto frameTime = std::chrono::steady_clock::now() - frameStart;
                if(frameTime < frameDuration)
                {
                    std::this_thread::sleep_for(frameDuration - frameTime);
                }
            }
        });

    screen.Loop(component);

    captureFinished = false;

    if(!output.empty())
    {
        if(output == "csv") stats.exportCsv();
        //else if(output == "json") exportJson();
        //
    }

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
