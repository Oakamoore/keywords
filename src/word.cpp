#include "word.h"
#include "random.h"

namespace Keywords
{
	Word::Word(std::string_view text, int verticalPos)
		: m_text {text}
		, m_y {verticalPos}
		, m_color {ftxui::Color::Green}
	{
		constexpr int minOffset {1};
		constexpr int maxOffset {50};

		// The starting position required for a word to spawn off screen
		int minHorizontalStartPos {-static_cast<int>(m_text.length() * 2)};

		m_x = minHorizontalStartPos - Random::get(minOffset, maxOffset);
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
		const int halfOfCanvas {static_cast<int>(canvasWidth * 0.50)};
		const int threeQuartersOfCanvas {static_cast<int>(canvasWidth * 0.75)};

		if (m_x >= threeQuartersOfCanvas)
			m_color = ftxui::Color::Red;
		else if (m_x >= halfOfCanvas)
			m_color = ftxui::Color::Yellow;
	}
}