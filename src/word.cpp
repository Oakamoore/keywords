#include "word.h"

namespace Keywords
{
	Word::Word(std::string_view text)
		: m_text {text}
		, m_color {ftxui::Color::Green}
	{
	}

	void Word::move()
	{
		m_x += s_speed;
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