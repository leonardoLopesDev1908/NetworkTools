#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


class ThreadPool
{
public:
    ThreadPool(std::size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();   
    void enqueue(std::function<void()> task);

private:
    bool m_stop = false;

    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::queue<std::function<void()>> m_taskQueue;
    std::vector<std::thread> m_pool;
};

#endif
