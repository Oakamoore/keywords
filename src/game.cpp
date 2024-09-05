#include "game.h"
#include "word_bank.h"
#include "input_component.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/loop.hpp>
#include <chrono>
#include <thread>
#include <filesystem>
#include <exception>

namespace
{
	void displayMainMenu(Keywords::SessionConfig& config /*std::function<void()> quit*/)
	{
		Keywords::InputComponent inputComponent {};

		auto component {Keywords::getMainMenuComponent(config, inputComponent)};
		auto screen {ftxui::ScreenInteractive::Fullscreen()};

		screen.Loop(component);
	}

	void displaySession(const Keywords::SessionConfig& config /*std::function<void()> quit*/)
	{
		Keywords::Session session {config};

		auto component {Keywords::getSessionComponent(session)};
		auto screen {ftxui::ScreenInteractive::Fullscreen()};

		ftxui::Loop loop {&screen, component | ftxui::center};

		while (!loop.HasQuitted())
		{
			loop.RunOnce();

			using namespace std::chrono_literals;

			const auto frameDuration {1.0s / 30.0};

			std::this_thread::sleep_for(frameDuration);

			// Force a screen update 
			screen.RequestAnimationFrame();

			session.update();
		}
	}

	// void displayLeaderboard();
}

namespace Keywords
{
	ftxui::Component getSessionComponent(const Session& session /*std::function<void()> quit*/)
	{
		auto component {ftxui::Renderer(session.getInputComponent(), [&]
		{
			{
				return session.draw();
			}
		})};

		return component;
	}

	void startGame()
	{
		const std::filesystem::path filePath {"resources/us-english-language-dictionary.txt"};

		try
		{
			WordBank::readFromFile(filePath);

			SessionConfig config {};

			config.difficulty = SessionConfig::Difficulty::easy;

			displayMainMenu(config);

			//displaySession(config);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what();
		}
	}
}