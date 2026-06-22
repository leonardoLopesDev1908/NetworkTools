#ifndef INTERCEPT_H
#define INTERCEPT_H

#include "Message.h"

#include <ftxui/component/screen_interactive.hpp>

#include <condition_variable>
#include <mutex>

class Intercept
{
  public:
    Message m_pending;

  public:
    std::optional<Message> wait(Message msg);
    void resolve(Message edited);
    Message* pending();
    void cancel();
    void setScreen(ftxui::ScreenInteractive* screen);

  private:
    ftxui::ScreenInteractive* m_screen = nullptr;

    bool m_cancelled = false;
    bool m_resolved = false;
    Message m_edited;

    std::mutex m_mutex;
    std::condition_variable m_cond;
};

#endif
