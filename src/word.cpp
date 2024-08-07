#include "word.h"
#include "random.h"

constexpr int g_minOffset {20};
constexpr int g_maxOffset {50};

namespace Keywords
{
	Word::Word(std::string_view text, int verticalPos)
		: m_text {text}
		, m_y {verticalPos}
		, m_x {-Random::get(g_minOffset, g_maxOffset)} // TODO: prevent overlapping?
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
		static const int halfOfCanvas {static_cast<int>(canvasWidth * 0.50)};
		static const int threeQuartersOfCanvas {static_cast<int>(canvasWidth * 0.75)};

		if (m_x >= threeQuartersOfCanvas)
			m_color = ftxui::Color::Red;
		else if (m_x >= halfOfCanvas)
			m_color = ftxui::Color::Yellow;
	}
}