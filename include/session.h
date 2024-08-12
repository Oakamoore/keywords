#ifndef KEYWORDS_SESSION_H
#define KEYWORDS_SESSION_H

#include "input_component.h"
#include "word.h"
#include "timer.h"
#include <ftxui/dom/node.hpp>
#include <ftxui/component/component.hpp>
#include <vector>
#include <string>

namespace Keywords
{
	struct SessionConfig
	{
		enum class Difficulty
		{
			easy,
			medium,
			hard,
			max_difficulty
		};
		
		Difficulty difficulty {};
	};

	class Session
	{
	public:
		Session(const SessionConfig& config);

		ftxui::Element draw();
		void update();

		ftxui::Component getInputComponent() { return m_input.getComponent(); }
		
	private:
		void addWords();
		void eraseWords();

	private:
		const SessionConfig m_config {};
		const std::vector<std::string>* m_wordBank {};
		Words m_words {}; 
		Timer m_uptime {};
		InputComponent m_input {};
		int m_misses {};
		int m_charsTyped {};
		int m_wordsTyped {};
		int m_wordsPerMinute {};
		double m_charsPerSecond {};

	};
}

#endif
