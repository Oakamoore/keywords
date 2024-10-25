#include "game.h"
#include "word_bank.h"
#include "input_component.h"
#include "main_menu.h"
#include "leaderboard.h"
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
			update();

			loop.RunOnce();

			using namespace std::chrono_literals;

			const auto frameDuration {1.0s / 30.0};

			std::this_thread::sleep_for(frameDuration);

			// Force a screen update
			// without interfering with events
			screen.RequestAnimationFrame();
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

	void displaySession(const Keywords::SessionConfig& config, const auto& back, const auto& lose)
	{
		auto screen {ftxui::ScreenInteractive::Fullscreen()};
		
		auto onBack {[&] { back(); screen.Exit(); }};
		auto onLose {[&] { lose(); screen.Exit(); }};

		Keywords::Session session {config, Keywords::Constants::saveFilePaths[config.difficulty], onBack, onLose};

		auto component {Keywords::getSessionComponent(session)};

		auto updateSession {[&] { session.update(); }};

		runCustomLoop(screen, component | ftxui::center, updateSession);
	}

	void displayLeaderboard(const Keywords::SessionConfig& config)
	{
		// Unflitered input 
		Keywords::InputComponent inputComponent {"Enter Username", false};

		auto screen {ftxui::ScreenInteractive::Fullscreen()};
		auto component {Keywords::getLeaderboardComponent(config, inputComponent)};

		auto quit {[&] { screen.Exit(); }};
		auto save {quit};

		auto updateLeaderboard {[&] { Keywords::Leaderboard::handleInput(inputComponent, Keywords::Constants::saveFilePaths[config.difficulty], quit, save); }};

		runCustomLoop(screen, component, updateLeaderboard);
	}
}

namespace Keywords
{
	ftxui::Component getSessionComponent(Session& session)
	{
		auto component {ftxui::Renderer(session.getInputComponent().component, [&]
		{
			{
				return session.draw();
			}
		})};

		return component;
	}

	void startGame()
	{

#if 1
		SessionConfig config {SessionConfig::hard};
		
		displayLeaderboard(config);
#endif

#if 0
		try
		{
			WordBank::readFromFile(Constants::wordList);

			SessionConfig config {};

			bool hasQuit {};
			auto quit {[&] { hasQuit = true; }};

			while (!hasQuit)
			{
				bool isPlaying {};
				auto play {[&] { isPlaying = true; }};
				auto back {[&] { isPlaying = false; }};

				displayMainMenu(config, quit, play);

				if (isPlaying)
				{
					bool hasLost {};
					auto lose {[&] { hasLost = true; isPlaying = false; }};

					try
					{
						displaySession(config, back, lose);
					
						if(hasLost) 
							displayLeaderboard(config);
					}
					catch (const std::exception& e)
					{
						std::cerr << "Error: " << e.what();
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what();
		}
#endif
	}
}