#ifndef KEYWORDS_CONSTANTS_H
#define KEYWORDS_CONSTANTS_H

#include "session.h"
#include <array>
#include <string_view>
#include <filesystem>

namespace Keywords
{
	namespace Constants
	{
		inline constexpr std::array<std::string_view, SessionConfig::difficultyCount> difficultyOptions 
		{
			"easy", 
			"medium", 
			"hard"
		};

		inline constexpr std::string_view playInput {"play"};
		inline constexpr char statSeparator {'|'};
		inline constexpr int maxMisses {10};
		
		inline const std::filesystem::path wordList {"resources/us-english-language-dictionary.txt"};
		inline const std::filesystem::path savedSessionStats {"resources/saved-session-stats.txt"};
	}
}

#endif