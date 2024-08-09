#ifndef KEYWORDS_SESSION_H
#define KEYWORDS_SESSION_H

#include "input_component.h"
#include "word.h"
#include "timer.h"
#include <ftxui/dom/node.hpp>
#include <ftxui/component/component.hpp>
#include <vector>

namespace Keywords
{
	class Session
	{
	public:
		Session(/*SessionConfig config*/);

		ftxui::Element draw();

		ftxui::Component getInputComponent() { return m_input.getComponent(); }
		
	private:
		void update();
		void addWords();
		void eraseWords();

	private:
		/*const SessionConfig m_config*/ 
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
