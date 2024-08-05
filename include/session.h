#ifndef KEYWORDS_SESSION_H
#define KEYWORDS_SESSION_H

#include "input_component.h"
#include <ftxui/dom/node.hpp>
#include <ftxui/component/component.hpp>

namespace Keywords
{
	class Session
	{
	public:
		Session() = default;

		// Draw 'm_words' to 'ftxui::Canvas'
		// Draw the stats of the 'Session' to 'ftxui::Canvas'
		ftxui::Element draw();

		ftxui::Component getInputComponent() { return m_input.getComponent(); }
		
	private:
		// Called in 'Session::draw()'
		// Reposition elements of 'm_words', in 'ftxui::Canvas'
		// This calls 'Session::addWords()' and 'Session::eraseWords()'
		void update();
		
		// Add new 'Word' objects to 'm_words'
		void addWords();
		
		// When 'ENTER' is pressed this checks against
		// 'm_words' and removes any 'Word' that matches
		void eraseWords();

	private:
		/*const SessionConfig m_config*/ 
		/*std::vector<Word> m_words*/
		/*Timer m_uptime*/
		
		InputComponent m_input;
		int m_misses {};
		int m_charsTyped {};
		int m_wordsPerMinute {};
		double m_charsPerSecond {};

	};
}

#endif
