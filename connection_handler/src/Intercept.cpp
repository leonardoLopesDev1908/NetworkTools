#include "Intercept.h"

std::optional<Message> Intercept::wait(Message msg)
{
    std::unique_lock lck(m_mutex);
    m_pending = std::move(msg);
    m_resolved = false;
    m_cancelled = false;

    if (m_screen)
        m_screen->PostEvent(ftxui::Event::Custom);

    m_cond.wait(lck, [&] { return m_resolved; });

    if (m_cancelled)
        return std::nullopt;

    return std::move(m_edited);
}

void Intercept::resolve(Message edited)
{
    std::lock_guard lck(m_mutex);
    m_edited = std::move(edited);
    m_resolved = true;
    m_cond.notify_one();
}

Message* Intercept::pending() { return m_resolved ? nullptr : &m_pending; }

void Intercept::cancel()
{
    std::unique_lock lck(m_mutex);
    m_cancelled = true;
    m_resolved = true;
    m_cond.notify_one();
}

void Intercept::setScreen(ftxui::ScreenInteractive* screen) { m_screen = screen; }
