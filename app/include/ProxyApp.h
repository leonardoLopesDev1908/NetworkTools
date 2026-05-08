#ifdef _WIN32
#include "ProxyWindows.h"
#elif __linux__
#include "ProxyLinux.h"
#endif

#include "ftxui/component/component.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

#include <future>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <thread>

using namespace ftxui;

class ProxyApp 
{
public:    
    void start();
    ftxui::Element manual_page();
    ftxui::Element input_endpoint_page();
    ftxui::Element messages_menu_page();
    ftxui::Element options_page();

private:
    enum class Language {English, Portuguese, Spanish};

    struct MessagesMenuState
    {
        //std::queue<Message> messages;
        std::vector<std::string> messages = {
            "a",  "c", "acae", "ascnoa"
        };
        int selectedMessage = 0;
    };

    struct OptionsState
    {
        std::vector<std::string> languages = { 
            "English", "Portuguese", "Spanish" 
        };
        int selectedLanguage = 0;
        bool keepMessagesFlag = false;
    };

    struct InputEndpointState 
    {
        std::string host;
        std::string port;
    };

private:
    int selected = 0;
    int activeTab = 0;

    InputEndpointState endpointState;
    MessagesMenuState messagesState;
    OptionsState optionsState;

    ftxui::Component menu;
    ftxui::Component inputHost;
    ftxui::Component inputPort;
    ftxui::Component btnSubmit;

    ftxui::Component main_container;
    ftxui::Component input_container;
    ftxui::Component menu_container;
    ftxui::Component messages_container;
    ftxui::Component options_container;

#ifdef _WIN32
    std::shared_ptr<ProxyWindows> m_proxy;
#elif __linux__
    std::shared_ptr<ProxyLinux> m_proxy;
#endif

    std::thread proxyThread;
};
