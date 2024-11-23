#include "transition.h"
#include "constants.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/canvas.hpp>
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

	void drawCanvasToScreen(ftxui::Screen& screen, ftxui::Canvas& canvas)
	{
		ftxui::Render(screen, ftxui::canvas(canvas));

		printScreen(screen);
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

		int maxY {screen.dimy() * Constants::canvasCellHeight};
		int maxX {screen.dimx() * Constants::canvasCellWidth};

		// Increment through and fill each canvas cell
		for (int x {0}; x <= screen.dimx(); ++x)
		{
			for (int y {0}; y <= maxY; ++y)
			{
				canvas.DrawBlock(x, y, true, ftxui::Color::Red);
				canvas.DrawBlock(maxX - x, y, true, ftxui::Color::Red);
			}

			drawCanvasToScreen(screen, canvas);
		}

		// Increment through and clear each canvas cell
		for (int x {screen.dimx()}; x >= 0; --x)
		{
			for (int y {0}; y <= maxY; ++y)
			{
				canvas.DrawBlock(x, y, true, ftxui::Color::Black);
				canvas.DrawBlock(maxX - x, y, true, ftxui::Color::Black);
			}

			drawCanvasToScreen(screen, canvas);
		}
	}
}