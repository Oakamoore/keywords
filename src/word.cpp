#include "word.h"

namespace Keywords
{
	Word::Word(std::string_view text, int canvasHeight)
		: m_text {text} // Ensure that this is lowercase
		, m_y {canvasHeight}
		, m_x {-static_cast<int>(text.length())}
		, m_color {ftxui::Color::Green}
	{

	}

	void Word::move([[maybe_unused]] int canvasWidth)
	{
		// Increments 'm_x' by 's_speed'

		// Calls 'updateColor(int)'
	}

	void Word::updateColor([[maybe_unused]] int canvasWidth)
	{
		// Get 1/3 of 'canvasWidth', when 'm_x' reaches that
		// change 'm_color' to 'ftxui::Color::Yellow'

		// Get 2/3 of 'canvasWidth', when 'm_x' reaches that 
		// change 'm_color' to 'ftxui::Color::Red'
	
	}
}