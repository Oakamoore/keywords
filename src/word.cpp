#include "word.h"
#include "random.h"

namespace Keywords
{
	Word::Word(std::string_view text, int verticalPos)
		: m_text {text}
		, m_y {verticalPos}
		, m_color {ftxui::Color::Green}
	{
		static constexpr int minOffset {1};
		static constexpr int maxOffset {50};

		// The minimum horizontal starting value 
		// required for a word to spawn off screen
		int clearance {-static_cast<int>(m_text.length() * 2)};

		m_x = clearance - Random::get(minOffset, maxOffset);
	}

	void Word::move(int canvasWidth)
	{
		if (m_x < canvasWidth)
		{
			m_x += s_speed;

			updateColor(canvasWidth);
		}
	}

	void Word::updateColor(int canvasWidth)
	{
		static const int halfOfCanvas {static_cast<int>(canvasWidth * 0.50)};
		static const int threeQuartersOfCanvas {static_cast<int>(canvasWidth * 0.75)};

		if (m_x >= threeQuartersOfCanvas)
			m_color = ftxui::Color::Red;
		else if (m_x >= halfOfCanvas)
			m_color = ftxui::Color::Yellow;
	}
}