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
#include <array>

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

			const auto frameDuration {1.0s / 15.0};

			std::this_thread::sleep_for(frameDuration);

			// Force a screen update
			// without interfering with events
			screen.RequestAnimationFrame();
		}
	}

	void displayMainMenu(Keywords::GameConfig& config, const auto& quit, const auto& play)
	{
		Keywords::InputComponent inputComponent {};

		auto component {Keywords::getMainMenuComponent(config, inputComponent)};
		auto screen {ftxui::ScreenInteractive::Fullscreen()};

		auto onQuit {[&] { quit(); screen.Exit(); }};
		auto onPlay {[&] { play(); screen.Exit(); }};

		auto updateMainMenu {[&] { Keywords::MainMenu::handleInput(config, inputComponent, onQuit, onPlay); }};

		Keywords::Audio::Track mainMenuTrack {Keywords::Constants::audioFilePaths[Keywords::Audio::main_menu]};
		
		if (config.isAudioEnabled) mainMenuTrack.play();

		runCustomLoop(screen, component, updateMainMenu);

		if (config.isAudioEnabled) mainMenuTrack.stop();
	}

	void displaySession(const Keywords::GameConfig& config, const Keywords::WordBank& wordBank, const auto& back, const auto& lose)
	{
		auto screen {ftxui::ScreenInteractive::Fullscreen()};
		
		auto onBack {[&] { back(); screen.Exit(); }};
		auto onLose {[&] { lose(); screen.Exit(); }};

		std::array<Keywords::Audio::Track, Keywords::Session::max_session_tracks> sessionTracks
		{
			Keywords::Constants::audioFilePaths[Keywords::Audio::session_slow],
			Keywords::Constants::audioFilePaths[Keywords::Audio::session_medium],
			Keywords::Constants::audioFilePaths[Keywords::Audio::session_fast]
		};

		Keywords::Session session {config, wordBank, Keywords::Util::getSaveFilePath(config.difficulty), sessionTracks, onBack, onLose};

		auto component {Keywords::getSessionComponent(session)};
		auto updateSession {[&] { session.update(); }};

		runCustomLoop(screen, component, updateSession);
	}

	void displayLeaderboard(const Keywords::GameConfig& config)
	{
		auto screen {ftxui::ScreenInteractive::Fullscreen()};
		auto onQuit {[&] { screen.Exit(); }};

		Keywords::Leaderboard leaderboard {config, Keywords::Util::getSaveFilePath(config.difficulty), onQuit};

		auto component {Keywords::getLeaderboardComponent(leaderboard)};
		auto updateLeaderboard {[&] { leaderboard.handleInput(); }};

		Keywords::Audio::Track leaderboardTrack {Keywords::Constants::audioFilePaths[Keywords::Audio::leaderboard]};
		
		if (config.isAudioEnabled) leaderboardTrack.play();

		runCustomLoop(screen, component, updateLeaderboard);

		if (config.isAudioEnabled) leaderboardTrack.stop();
	}
}

namespace Keywords
{
	void startGame(int argc, char* argv[])
	{
		try
		{
			WordBank wordBank {Constants::wordBankPaths};
			GameConfig config {!Util::isArgumentPresent(argc, argv, Constants::disableAudio)};

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
						displaySession(config, wordBank, back, lose);

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