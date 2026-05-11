#ifndef QUEUE_MESSAGE_H
#define QUEUE_MESSAGE_H

#include "Message.h"

#include <ftxui/component/screen_interactive.hpp>

#include <atomic>
#include <deque>
#include <mutex>

class QueueMessage
{   
public:
    void push(Message msg);            
    void tryPush(Message&& msg);
    Message pop();
    Message tryPop();
    Message front();
    bool empty() const;

    void setScreen(ftxui::ScreenInteractive* screen);
    std::deque<Message> snapshot();

private:
    ftxui::ScreenInteractive* m_screen = nullptr;

    std::deque<Message> m_messages;
    
    std::mutex m_queueMutex;
    std::mutex m_screenMutex;
};

#endif
