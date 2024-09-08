#include "game.h"
#include "word_bank.h"
#include "input_component.h"
#include "main_menu.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/loop.hpp>
#include <chrono>
#include <thread>
#include <filesystem>
#include <exception>
#include <functional>

namespace
{
	void runCustomLoop(ftxui::ScreenInteractive& screen,
					   ftxui::Component component,
					   const auto& update)
	{
		ftxui::Loop loop {&screen, component};

		while(!loop.HasQuitted())
		{
			loop.RunOnce();

			using namespace std::chrono_literals;

			const auto frameDuration {1.0s / 30.0};

			std::this_thread::sleep_for(frameDuration);

			// Force a screen update
			// without interfering with events
			screen.RequestAnimationFrame();

			update();
		}
	}

	void displayMainMenu(Keywords::SessionConfig& config, const auto& quit)
	{
		Keywords::InputComponent inputComponent {};

		auto component {Keywords::getMainMenuComponent(config, inputComponent)};
		auto screen {ftxui::ScreenInteractive::Fullscreen()};

		auto exit {[&] { quit(); screen.Exit(); }};

		auto updateMainMenu {[&] { Keywords::MainMenu::handleInput(config, inputComponent, exit); }};

		runCustomLoop(screen, component, updateMainMenu);
	}

	void displaySession(const Keywords::SessionConfig& config /*std::function<void()> quit*/)
	{
		Keywords::Session session {config};

		auto component {Keywords::getSessionComponent(session)};
		auto screen {ftxui::ScreenInteractive::Fullscreen()};

		auto updateSession {[&] { session.update(); }};

		runCustomLoop(screen, component | ftxui::center, updateSession);
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

			bool hasQuit {false};
			auto quit {[&] { hasQuit = true; }};

			while (!hasQuit)
			{
				displayMainMenu(config, quit);

				//displaySession(config);
			}

		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what();
		}
	}
}