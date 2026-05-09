#include "QueueMessage.h"

void QueueMessage::push(Message msg)
{
    m_messages.push(msg);
}

void QueueMessage::tryPush(Message msg)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_cond.wait(lck, [&]{ return isSafe;});
    isSafe = false;
    m_messages.push(msg);
    isSafe = true;
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
    m_cond.wait(lck, [&]{ return isSafe;});

    isSafe = false;
    Message msg = m_messages.front();
    m_messages.pop();
    isSafe = true;

    m_cond.notify_all();

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

