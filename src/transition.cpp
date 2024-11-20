#include "transition.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/canvas.hpp>
#include <iostream>

namespace
{
	constexpr int g_screenClearance {50};
	constexpr int g_radiusModifier {2};

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

	void fillScreen(ftxui::Screen& screen, ftxui::Canvas& canvas)
	{
		int radius {};

		while (radius < screen.dimx() + g_screenClearance)
		{
			// A filled circle increasing in size
			canvas.DrawBlockCircleFilled(screen.dimx(), screen.dimy() * 2, radius, ftxui::Color::Red);

			drawCanvasToScreen(screen, canvas);

			radius += g_radiusModifier;
		}
	}

	void reverseFillScreen(ftxui::Screen& screen, ftxui::Canvas& canvas)
	{
		int radius {screen.dimx() + g_screenClearance};

		while (radius > 0)
		{
			// An unfilled circle decreasing in size
			canvas.DrawBlockCircle(screen.dimx(), screen.dimy() * 2, radius, ftxui::Color::Black);

			drawCanvasToScreen(screen, canvas);

			radius -= g_radiusModifier;
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

		fillScreen(screen, canvas);
		reverseFillScreen(screen, canvas);
	}
}