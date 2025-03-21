#include "util.h"
#include "constants.h"
#include <algorithm>

namespace Keywords
{
	namespace Util
	{
		std::string_view getStringFromDifficulty(GameConfig::Difficulty difficulty)
		{
			using enum Keywords::GameConfig::Difficulty;

			switch (difficulty)
			{
				case medium:	return	Constants::difficultyOptions[medium];
				case hard:		return	Constants::difficultyOptions[hard];
				default:		return	Constants::difficultyOptions[easy];
			}
		}

		const std::filesystem::path& getSaveFilePath(GameConfig::Difficulty difficulty)
		{
			using enum Keywords::GameConfig::Difficulty;

			switch (difficulty)
			{
				case medium:	return Constants::saveFilePaths[medium];
				case hard:		return Constants::saveFilePaths[hard];
				default:		return Constants::saveFilePaths[easy];
			}
		}

		std::string convertToCase(std::string_view str, std::function<int(int)> conversion)
		{
			if (!str.empty())
			{
				std::string output(str.length(), ' ');

				std::transform(str.begin(), str.end(), output.begin(), [&] (unsigned char c)
				{
					return static_cast<char>(conversion(c));
				});

				return output;
			}

			return {};
		}

		bool isArgumentPresent(int argc, char* argv[], std::string_view expectedArgument)
		{
			if (argc <= 1)
				return false;

			// Skip the first argument
			for (int i {1}; i < argc; ++i)
			{
				if (argv[i] == expectedArgument)
					return true;
			}

			return false;
		}
	}
}