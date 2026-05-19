#include "Intercept.h"

Message Intercept::wait(Message msg)
{
	std::unique_lock lck(m_mutex);
	m_pending = std::move(msg);
	resolved = false;
	
	if (m_screen)
		m_screen->PostEvent(ftxui::Event::Custom);

	m_cond.wait(lck, [&] {return resolved;});
	return std::move(m_edited);
}

void Intercept::resolve(Message edited)
{
	std::lock_guard lck(m_mutex);
	m_edited = std::move(edited);
	resolved = true;
	m_cond.notify_one();
}

Message* Intercept::pending()
{
	std::lock_guard lck(m_mutex);
	return resolved ? nullptr : &m_pending;
}

void Intercept::setScreen(ftxui::ScreenInteractive* screen)
{
	m_screen = screen;
}

