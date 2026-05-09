#ifndef QUEUE_MESSAGE_H
#define QUEUE_MESSAGE_H

#include "Message.h"

#include <condition_variable>
#include <queue>

class QueueMessage
{
    QueueMessage(std::size_t size);
    
    void push(Message msg);            
    void tryPush(Message msg);
    Message pop();
    Message tryPop();
    Message front();

private:
    std::queue<Message> m_messages;
    std::mutex qMutex;
    std::condition_variable m_cond;
};


#define
