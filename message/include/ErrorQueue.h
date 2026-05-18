#ifndef ERROR_QUEUE_H
#define ERROR_QUEUE_H

#include <ftxui/component/screen_interactive.hpp>

#include <mutex>
#include <optional>
#include <queue>

class ErrorQueue
{
public:

	void push(const std::string& error);
	std::optional<std::string> pop();
	void setScreen(ftxui::ScreenInteractive* screen);

private:
	ftxui::ScreenInteractive* m_screen = nullptr;
	std::queue<std::string> m_errors;
	std::mutex m_mutex;
};

#endif

