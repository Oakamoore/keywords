#ifndef KEYWORDS_UTIL_H
#define KEYWORDS_UTIL_H

#include "game_config.h"
#include <string>
#include <string_view>
#include <functional>
#include <filesystem>

namespace Keywords
{
	namespace Util
	{
		std::string_view getStringFromDifficulty(GameConfig::Difficulty difficulty);
		const std::filesystem::path& getSaveFilePath(GameConfig::Difficulty difficulty);
		std::string convertToCase(std::string_view str, std::function<int(int)> conversion);
		bool isArgumentPresent(int argc, char* argv[], std::string_view expectedArgument);
	}
}

#endif