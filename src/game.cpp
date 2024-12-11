#include "game.h"
#include "word_bank.h"
#include "input_component.h"
#include "main_menu.h"
#include "leaderboard.h"
#include "constants.h"
#include "util.h"
#include "transition.h"
#include "audio.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/loop.hpp>
#include <chrono>
#include <thread>
#include <exception>

namespace
{
	void runCustomLoop(ftxui::ScreenInteractive& screen,
					   ftxui::Component component,
					   const auto& update,
					   Keywords::Audio::TrackID trackID)
	{
		ftxui::Loop loop {&screen, component};

		Keywords::Audio::Track track {Keywords::Constants::audioFilePaths[trackID]};

		track.play();

		while(!loop.HasQuitted())
		{
			update();

			loop.RunOnce();

			using namespace std::chrono_literals;

			const auto frameDuration {1.0s / 15.0};

			std::this_thread::sleep_for(frameDuration);

			// Force a screen update
			// without interfering with events
			screen.RequestAnimationFrame();
		}

		track.stop();
	}

	void displayMainMenu(Keywords::SessionConfig& config, const auto& quit, const auto& play)
	{
		Keywords::InputComponent inputComponent {};

		auto component {Keywords::getMainMenuComponent(config, inputComponent)};
		auto screen {ftxui::ScreenInteractive::Fullscreen()};

		auto onQuit {[&] { quit(); screen.Exit(); }};
		auto onPlay {[&] { play(); screen.Exit(); }};

		auto updateMainMenu {[&] { Keywords::MainMenu::handleInput(config, inputComponent, onQuit, onPlay); }};

		runCustomLoop(screen, component, updateMainMenu, Keywords::Audio::main_menu);
	}

	void displaySession(const Keywords::SessionConfig& config, const auto& back, const auto& lose)
	{
		auto screen {ftxui::ScreenInteractive::Fullscreen()};
		
		auto onBack {[&] { back(); screen.Exit(); }};
		auto onLose {[&] { lose(); screen.Exit(); }};

		Keywords::Session session {config, Keywords::Util::getFilePathFromDifficulty(config.difficulty), onBack, onLose};

		auto component {Keywords::getSessionComponent(session)};
		auto updateSession {[&] { session.update(); }};

		runCustomLoop(screen, component, updateSession, Keywords::Audio::session);
	}

	void displayLeaderboard(const Keywords::SessionConfig& config)
	{
		auto screen {ftxui::ScreenInteractive::Fullscreen()};
		auto onQuit {[&] { screen.Exit(); }};

		Keywords::Leaderboard leaderboard {config, Keywords::Util::getFilePathFromDifficulty(config.difficulty), onQuit};

		auto component {Keywords::getLeaderboardComponent(leaderboard)};
		auto updateLeaderboard {[&] { leaderboard.handleInput(); }};

		runCustomLoop(screen, component, updateLeaderboard, Keywords::Audio::leaderboard);
	}
}

namespace Keywords
{
	void startGame()
	{
		try
		{
			WordBank::readFromFile(Constants::wordBankFilePath);

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

						if (hasLost)
						{
							displayTransition();
							displayLeaderboard(config);
						}
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
	}
}