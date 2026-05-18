#include "ErrorQueue.h"

void ErrorQueue::push(const std::string& error)
{
	{
		std::lock_guard lck(m_mutex);
		m_errors.push(error);
	}

	if (m_screen)
		m_screen->PostEvent(ftxui::Event::Custom);
}

std::optional<std::string> ErrorQueue::pop()
{
	std::lock_guard lck(m_mutex);
	if (m_errors.empty())
		return std::nullopt;
	auto e = m_errors.front();
	m_errors.pop();
	return e;
}

void ErrorQueue::setScreen(ftxui::ScreenInteractive* screen)
{
	m_screen = screen;
}