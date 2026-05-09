#include "QueueMessage.h"

QueueMessage::QueueMessage(std::size_t size)
{
}

void QueueMessage::push(Message msg)
{
    m_messages.push(msg);
}

void QueueMessage::tryPush(Message msg)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_cond.wait([&]{ return !isSafe;});
    m_messages.push(msg);
    m_cond.notify_all();
}

Message QueueMessage::pop()
{
    Message msg = m_messages.front();
    m_messages.pop();
    return msg;
}

Message QueueMessage::tryPop()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_cond.wait([&]{ return !isSafe;});
    
    Message msg = m_messages.front();
    m_messages.pop();
    m_cond.notify_all();

    return msg;
}

bool QueueMessage::empty() const
{
    return m_messages.empty();
}

