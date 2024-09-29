#include "game.h"
#include "word_bank.h"
#include "input_component.h"
#include "main_menu.h"
#include "constants.h"
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

	void displayMainMenu(Keywords::SessionConfig& config, const auto& quit, const auto& play)
	{
		Keywords::InputComponent inputComponent {};

		auto component {Keywords::getMainMenuComponent(config, inputComponent)};
		auto screen {ftxui::ScreenInteractive::Fullscreen()};

		auto onQuit {[&] { quit(); screen.Exit(); }};
		auto onPlay {[&] { play(); screen.Exit(); }};

		auto updateMainMenu {[&] { Keywords::MainMenu::handleInput(config, inputComponent, onQuit, onPlay); }};

		runCustomLoop(screen, component, updateMainMenu);
	}

	void displaySession(const Keywords::SessionConfig& config, const auto& back /*const auto& lose*/)
	{
		auto screen {ftxui::ScreenInteractive::Fullscreen()};
		
		auto onBack {[&] { back(); screen.Exit(); }};
		// auto onLose {[&] { lose(); screen.Exit(); };

		Keywords::Session session {config, onBack};

		auto component {Keywords::getSessionComponent(session)};

		auto updateSession {[&] { session.update(); }};

		runCustomLoop(screen, component | ftxui::center, updateSession);
	}

	// void displayLeaderboard();
}

namespace Keywords
{
	ftxui::Component getSessionComponent(const Session& session)
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
		try
		{
			WordBank::readFromFile(Constants::filePath);

			SessionConfig config {};

			bool hasQuit {};
			auto quit {[&] { hasQuit = true; }};

			while (!hasQuit)
			{
				bool isPlaying {};
				auto play {[&] { isPlaying = true; }};

				displayMainMenu(config, quit, play);

				if (isPlaying)
				{
					auto back {[&] { isPlaying = false; }};

					displaySession(config, back);
				}

				/*if(!isPlaying && hasLost)
				{
					displayLeaderboard();
				}
				*/
			}

		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what();
		}
	}
}