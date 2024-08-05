#include "session.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

namespace
{
	constexpr int g_canvasWidth {120};
	constexpr int g_canvasHeight {100};

	/*int calcuateWordsPerMinute()*/
	/*int calcuateCharsPerSecond()*/
}

namespace Keywords
{
	ftxui::Element Session::draw()
	{
		// Create a temporary blank 'ftxui::Canvas' 
		ftxui::Canvas c {g_canvasWidth, g_canvasHeight};

		c.DrawText(0, 0, std::to_string(m_input.hasPressedEnter()), ftxui::Color::White);

		return ftxui::vbox({
			ftxui::canvas(std::move(c)) | ftxui::border,
			m_input.draw() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, g_canvasWidth)
						   });

		// Call 'Session::update()' post draw phase

	}

	void Session::update()
	{
		// Loops through 'm_words', and increments their positions
		// Calls 'addWords()' and 'eraseWords()' (the latter only on 'ENTER')


	}

	void Session::addWords()
	{
		// Reads in from 'WordBank', then creates new 'Word' object
		// Adds a given amount of 'Word' objects to 'm_words'


	}

	void Session::eraseWords()
	{
		// Loops through 'm_words' and checks against 'Word::m_content'
		// if they match 'InputComponent::getContent()', they are erased


	}
}