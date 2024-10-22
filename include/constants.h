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

		inline constexpr int maxMisses {10};
		inline constexpr std::string_view playInput {"play"};
		inline const auto wordDeletionEvent {ftxui::Event::Special("\x17")}; // CTRL + W 
		inline constexpr char statSeparator {'|'};
		
		inline const std::filesystem::path wordList {"resources/us-english-language-dictionary.txt"};
		inline const std::filesystem::path savedSessionStats {"resources/saved-session-stats.txt"};
	}
}

#endif