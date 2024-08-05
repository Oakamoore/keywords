#include "session.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

namespace
{
	constexpr int g_canvasWidth {240};
	constexpr int g_canvasHeight {100};

	/*constexpr int g_maxMisses {10};*/

	/*int calcuateWordsPerMinute()*/
	/*int calcuateCharsPerSecond()*/
}

namespace Keywords
{
	ftxui::Element Session::draw()
	{
		// Create a temporary blank 'ftxui::Canvas' 
		ftxui::Canvas c {g_canvasWidth, g_canvasHeight};

		// TODO: Loop through 'm_words' and draw each to the 'ftxui::Canvas'

		// TODO: Call 'Session::update()' post draw phase

		return ftxui::vbox({
			ftxui::text("KEYWORDS") | ftxui::center, 
			ftxui::separatorEmpty(),
			ftxui::canvas(std::move(c)) | ftxui::border,
			m_input.draw() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, g_canvasWidth / 5),
			ftxui::separatorEmpty()
						   });
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
		// Ensure all the height values are different


	}

	void Session::eraseWords()
	{
		// Loops through 'm_words' and checks against 'Word::m_content'
		// if they match 'InputComponent::getContent()', they are erased


	}
}