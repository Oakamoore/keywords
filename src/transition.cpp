#include "transition.h"
#include "constants.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/terminal.hpp>
#include <iostream>

namespace
{
	void printScreen(ftxui::Screen& screen)
	{
		static std::string resetPosition {};

		// Prevents the terminal from scrolling
		std::cout << "\033[?25l" << resetPosition << screen.ToString() << std::flush;
		resetPosition = screen.ResetPosition();
	}

	void fillScreen(ftxui::Screen& screen)
	{
		// Increment through and fill each screen pixel
		for (int x {0}; x <= screen.dimx() / 2; ++x)
		{
			for (int y {0}; y <= screen.dimy(); ++y)
			{
				screen.PixelAt(x, y).background_color = ftxui::Color::Red;
				screen.PixelAt(screen.dimx() - x, y).background_color = ftxui::Color::Red;
			}

			printScreen(screen);
		}
	}

	void clearScreen(ftxui::Screen& screen)
	{
		// Increment through and clear each screen pixel
		for (int x {screen.dimx() / 2}; x >= 0; --x)
		{
			for (int y {0}; y <= screen.dimy(); ++y)
			{
				screen.PixelAt(x, y).background_color = ftxui::Color::Default;
				screen.PixelAt(-x + screen.dimx(), y).background_color = ftxui::Color::Default;
			}

			printScreen(screen);
		}
	}

}

namespace Keywords
{
	void displayTransition()
	{
		constexpr int canvasWidth {1000};
		constexpr int canvasHeight {1000};

		auto screen {ftxui::Screen::Create(ftxui::Dimension::Full())};
		auto canvas {ftxui::Canvas {canvasWidth, canvasHeight}};

		fillScreen(screen);
		clearScreen(screen);
	}
}