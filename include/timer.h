#ifndef KEYWORDS_TIMER_H
#define KEYWORDS_TIMER_H

#include <chrono>

namespace Keywords
{
	class Timer
	{
	public:
		Timer() = default;

		using Clock = std::chrono::steady_clock;
		using Second = std::chrono::duration<double, std::ratio<1>>;

		double elapsed() const { return std::chrono::duration_cast<Second>(Clock::now() - m_currentTime).count(); }

	private:
		std::chrono::time_point<Clock> m_currentTime {Clock::now()};

	};
}

#endif