#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t numThreads)
{
    for (std::size_t i = 0; i < numThreads; ++i)
    {
        m_pool.emplace_back(
            [this]
            {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lck(m_mutex);
                        m_cond.wait(lck, [&] { return !m_taskQueue.empty() || m_stop; });

                        if (m_stop && m_taskQueue.empty())
                            return;

                        task = std::move(m_taskQueue.front());
                        m_taskQueue.pop();
                    }
                    task();
                }
            });
    }
}

ThreadPool::~ThreadPool()
{
    stop();

    for (auto& t : m_pool)
    {
        if (t.joinable())
            t.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_taskQueue.push(std::move(task));
    }

    m_cond.notify_one();
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_stop = true;
    }
    m_cond.notify_all();
}
