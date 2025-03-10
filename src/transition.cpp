#include "transition.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/terminal.hpp>
#include <iostream>

namespace
{
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

			screen.Print();
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

			screen.Print();
		}
	}

}

namespace Keywords
{
	void displayTransition()
	{
		auto screen {ftxui::Screen::Create(ftxui::Dimension::Full())};

		fillScreen(screen);
		clearScreen(screen);
	}
}