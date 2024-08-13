#include "word.h"
#include <ftxui/screen/color.hpp>
#include <catch2/catch_test_macros.hpp>

// An arbitrary canvas width
constexpr int g_canvasWidth {100};

TEST_CASE("Stop moving once the horizontal boundary has been reached")
{
	Keywords::Word word {"word", 0, 0};

	// Move the word up until the horizontal boundary
	while (word.getX() < g_canvasWidth)
		word.move(g_canvasWidth);

	auto previousHorizontalPos {word.getX()};

	// Attempt to move past the horizontal boundary
	word.move(g_canvasWidth);

	// The word shouldn't have moved
	REQUIRE(word.getX() == previousHorizontalPos);
}

TEST_CASE("Change color depending on relative horizontal position")
{
	constexpr int halfOfCanvas {static_cast<int>(g_canvasWidth * 0.50)};
	constexpr int threeQuartersOfCanvas {static_cast<int>(g_canvasWidth * 0.75)};

	Keywords::Word word {"word", 0, 0};

	while (word.getX() < g_canvasWidth)
	{
		word.move(g_canvasWidth);

		// A word's color should change to reflect 
		// its horizontal position in a given canvas 
		if (word.getX() >= threeQuartersOfCanvas)
			REQUIRE(word.getColor() == ftxui::Color::Red);
		else if (word.getX() >= halfOfCanvas)
			REQUIRE(word.getColor() == ftxui::Color::Yellow);
		else
			REQUIRE(word.getColor() == ftxui::Color::Green);
	}
}
