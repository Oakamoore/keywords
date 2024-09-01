#include "word.h"
#include <ftxui/screen/color.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Change color depending on relative horizontal position")
{
	// An arbitrary canvas width
	constexpr int canvasWidth {100};

	constexpr int halfOfCanvas {static_cast<int>(canvasWidth * 0.50)};
	constexpr int threeQuartersOfCanvas {static_cast<int>(canvasWidth * 0.75)};

	Keywords::Word word {"word"};

	while (word.x < canvasWidth)
	{
		word.move();
		word.updateColor(canvasWidth);

		// A word's color should change to reflect 
		// its horizontal position in a given canvas 
		if (word.x >= threeQuartersOfCanvas)
			REQUIRE(word.color == ftxui::Color::Red);
		else if (word.x >= halfOfCanvas)
			REQUIRE(word.color == ftxui::Color::Yellow);
		else
			REQUIRE(word.color == ftxui::Color::Green);
	}
}
