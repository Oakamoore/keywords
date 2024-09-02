#include "session.h"
#include "word_bank.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>

int main()
{
#if 1

	// Put this in 'constants.h' alongside 'const std::string_view quit {"quit"}'
	const std::filesystem::path filePath {"resources/us-english-language-dictionary.txt"};

	try
	{
		Keywords::WordBank::readFromFile(filePath);

		auto screen {ftxui::ScreenInteractive::Fullscreen()};

		Keywords::SessionConfig config {Keywords::SessionConfig::Difficulty::easy};

		Keywords::Session session {config};

		auto component {ftxui::Renderer(session.getInputComponent(), [&]
			{
				return session.draw();
			}
		)};

		ftxui::Loop loop {&screen, component | ftxui::center};

		while (!loop.HasQuitted())
		{
			loop.RunOnce();

			using namespace std::chrono_literals;

			const auto frameDuration {1.0s / 30.0};

			// TODO: Create a seperate thread for this, since this slows down input events
			std::this_thread::sleep_for(frameDuration);

			// Force a screen update 
			screen.RequestAnimationFrame();

			session.update();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what();
	}

#endif

	return 0;
}