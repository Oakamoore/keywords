#include "word.h"
#include "random.h"

namespace Keywords
{
	Word::Word(std::string_view text, int x, int y)
		: m_text {text}
		, m_x {x}
		, m_y {y}
		, m_color {ftxui::Color::Green}
	{
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