#ifndef QUEUE_MESSAGE_H
#define QUEUE_MESSAGE_H

#include "Message.h"

#include <condition_variable>
#include <queue>

class QueueMessage
{   
    void push(Message msg);            
    void tryPush(Message msg);
    Message pop();
    Message tryPop();
    Message front();
    bool empty() const;

private:
    bool isSafe = true;

    std::queue<Message> m_messages;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};

#endif
