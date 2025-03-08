#ifndef KEYWORDS_CONSTANTS_H
#define KEYWORDS_CONSTANTS_H

#include "game_config.h"
#include "audio.h"
#include <ftxui/component/event.hpp>
#include <array>
#include <string_view>
#include <filesystem>

namespace Keywords
{
	namespace Constants
	{
		inline constexpr std::array<std::string_view, GameConfig::max_difficulty> difficultyOptions
		{
			"easy",
			"medium",
			"hard"
		};

		inline const std::array<std::filesystem::path, GameConfig::max_difficulty> saveFilePaths
		{
			"resources/save_files/easy_session_save_file.txt",
			"resources/save_files/medium_session_save_file.txt",
			"resources/save_files/hard_session_save_file.txt"
		};

		inline const std::array<std::filesystem::path, Audio::TrackID::max_track_id> audioFilePaths
		{
			"resources/audio/main_menu_track.wav",
			"resources/audio/session_slow_track.wav",
			"resources/audio/session_medium_track.wav",
			"resources/audio/session_fast_track.wav",
			"resources/audio/leaderboard_track.wav"
		};

		inline const std::array<std::filesystem::path, GameConfig::max_difficulty> wordBankPaths
		{
			"resources/word_bank/easy_words.txt",
			"resources/word_bank/medium_words.txt",
			"resources/word_bank/hard_words.txt"
		};

		// The dimensions of an 'ftxui::Canvas' cell
		inline constexpr int canvasCellWidth {2};
		inline constexpr int canvasCellHeight {4};

		inline constexpr int maxMisses {10};

		inline constexpr std::string_view playInput {"play"};
		inline constexpr std::string_view disableAudio {"no-audio"};

		inline constexpr char statSeparator {'|'};
		inline const auto wordDeletionEvent {ftxui::Event::Special("\x1B[3~")}; // DELETE KEY
	}
}

#endif