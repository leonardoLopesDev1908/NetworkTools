#include "Proxy.h"

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


class MainApp 
{
public:    
    ~MainApp();
    void start();
    ftxui::Element manual_page();
    ftxui::Element input_endpoint_page();
    ftxui::Element messages_menu_page();
    ftxui::Element options_page();

private:
    ftxui::Element edit_messages_menu();
    ftxui::Element show_messages_menu(std::deque<Message> messages);

    enum class Language {English, Portuguese, Spanish};

    struct MessagesMenuState
    {
        std::vector<std::string> m_messageEntries;
        int selectedMessage = 0;
    };
    
    struct EditState
    {
        std::string method;
        std::string path;
        std::string body;
        ftxui::Component inputMethod;
        ftxui::Component inputPath;
        ftxui::Component inputBody;
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
    std::string m_submitError;
    Message* m_pending = nullptr;
    Message m_editMessage;

    InputEndpointState endpointState;
    MessagesMenuState messagesState;
    EditState editState;
    OptionsState optionsState;

    ftxui::Component menu;
    ftxui::Component inputHost;
    ftxui::Component inputPort;
    ftxui::Component methodInput;
    ftxui::Component pathInput;
    ftxui::Component bodyText;

    ftxui::Component btnSubmit;
    ftxui::Component btnEndProxy;
    ftxui::Component btnForward;

    ftxui::Component main_container;
    ftxui::Component input_container;
    ftxui::Component menu_container;
    ftxui::Component messages_container;
    ftxui::Component edit_messages_container;
    ftxui::Component options_container;

    std::shared_ptr<Proxy> m_proxy;
    std::thread proxyThread;
};
