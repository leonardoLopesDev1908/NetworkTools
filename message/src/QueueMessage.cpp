#include "QueueMessage.h"

#include <fstream>

void QueueMessage::push(Message msg)
{
    m_messages.push_back(msg);
}

void QueueMessage::tryPush(Message&& msg)
{
    std::cout << "[Info]: Message received at queue\n";
    {
        std::unique_lock<std::mutex> lck(m_queueMutex);
<<<<<<< Updated upstream
        std::cout << "[Info]: Message receivedn\n";
=======
>>>>>>> Stashed changes
        m_messages.push_back(std::move(msg));
    }

    if (m_screen)
        m_screen->PostEvent(ftxui::Event::Custom);
}

Message QueueMessage::pop()
{
    Message msg = m_messages.front();
    m_messages.pop_front();
    return msg;
}

Message QueueMessage::tryPop()
{
    std::unique_lock<std::mutex> lck(m_queueMutex);
    Message msg = m_messages.front();
    m_messages.pop_front();
    return msg;
}

Message QueueMessage::front()
{
    return m_messages.front();
}

bool QueueMessage::empty() const
{
    return m_messages.empty();
}

void QueueMessage::setScreen(ftxui::ScreenInteractive* screen)
{
    m_screen = screen;
}

std::deque<Message> QueueMessage::snapshot()
{
    return m_messages;
}
