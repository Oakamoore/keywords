#include "word.h"

namespace Keywords
{
	Word::Word(std::string_view text)
		: text {text}
	{
	}

	void Word::move()
	{
		constexpr int speed {1};

		x += speed;
	}

	void Word::updateColor(int canvasWidth)
	{
		const int halfOfCanvas {static_cast<int>(canvasWidth * 0.50)};
		const int threeQuartersOfCanvas {static_cast<int>(canvasWidth * 0.75)};

		if (x >= threeQuartersOfCanvas)
			color = ftxui::Color::Red;
		else if (x >= halfOfCanvas)
			color = ftxui::Color::Yellow;
	}
}