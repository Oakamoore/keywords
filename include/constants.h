#ifndef KEYWORDS_CONSTANTS_H
#define KEYWORDS_CONSTANTS_H

#include "session.h"
#include <array>
#include <string_view>
#include <filesystem>
#include <ftxui/component/event.hpp>

namespace Keywords
{
	namespace Constants
	{
		inline constexpr std::array<std::string_view, SessionConfig::max_difficulty> difficultyOptions
		{
			"easy",
			"medium",
			"hard"
		};

		inline const std::array<std::filesystem::path, SessionConfig::max_difficulty> saveFilePaths
		{
			"resources/save_files/easy_session_save_file.txt",
			"resources/save_files/medium_session_save_file.txt",
			"resources/save_files/hard_session_save_file.txt"
		};

		// The dimensions of an 'ftxui::Canvas' cell
		inline constexpr int canvasCellWidth {2};
		inline constexpr int canvasCellHeight {4};

		inline constexpr int maxMisses {10};
		inline constexpr std::string_view playInput {"play"};
		inline const auto wordDeletionEvent {ftxui::Event::Special("\x17")}; // CTRL + W 
		inline constexpr char statSeparator {'|'};
		
		inline const std::filesystem::path wordList {"resources/word_bank/us_english_language_dictionary.txt"};
	}
}

#endif