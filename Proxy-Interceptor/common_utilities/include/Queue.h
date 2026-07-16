#ifndef QUEUE_H
#define QUEUE_H

#include <ftxui/component/screen_interactive.hpp>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

template <typename T> class Queue
{
    ftxui::ScreenInteractive* m_screen = nullptr;

    std::deque<T> m_items;

    std::condition_variable m_queueCond;
    std::mutex m_queueMutex;
    std::mutex m_screenMutex;

public:
    void tryPush(T&& t)
    {
        {
            std::lock_guard lck(m_queueMutex);
            m_items.push_back(std::move(t));
        }
        if (m_screen)
            m_screen->PostEvent(ftxui::Event::Custom);
    }

    std::optional<T> tryPop()
    {
        std::unique_lock<std::mutex> lck(m_queueMutex);

        if (m_items.empty())
            return std::nullopt;

        T t = std::move(m_items.front());
        m_items.pop_front();
        return t;
    }

    T front()
    {
        std::unique_lock<std::mutex> lck(m_queueMutex);
        m_queueCond.wait(lck, [&] { return !m_items.empty(); });
        return m_items.front();
    }

    bool empty() const
    {
        std::lock_guard lck(m_queueMutex);
        return m_items.empty();
    }

    void setScreen(ftxui::ScreenInteractive* screen) { m_screen = screen; }

    std::deque<T> snapshot() { return m_items; }
};

#endif
