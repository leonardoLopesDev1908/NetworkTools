#include "ThreadPool"

ThreadPool::ThreadPool(std::size_t numThreads = std::thread::hardware_concurrency())
{
    for(std::size_t i = 0; i < numThreads; ++i)
    {
        m_pool.emplace_back([this] {
            while(true)
            {
                std::funcion<void> task;
                {
                    std::unique_lock<std::mutex> lck(m_mutex);

                    m_cond.wait([&]{!m_pool.empty() || !m_stop;});

                    if(m_stop && m_pool.empty())
                        return;

                    task = std::move(m_pool.front();
                    m_pool.pop();
                }
                task();
            }
        }); 
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex>lck (m_mutex);
        m_stop = true;
    }

    if(m_stop && m_pool.empty())
        return;
    
    m_cond.notify_all();

    for(auto& t : m_pool)
        t.join();
}

void ThreadPool::enqueue(std::function<void> task)
{
    {
        std::unique_lock<std::mutex>lck (m_mutex);
        m_queue.push(std::move(task));
    }
    
    m_cond.notify_one();
}

