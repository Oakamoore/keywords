#include "util.h"
#include "constants.h"
#include <algorithm>

namespace Keywords
{
	namespace Util
	{
		std::string_view getStringFromDifficulty(SessionConfig::Difficulty difficulty)
		{
			using enum Keywords::SessionConfig::Difficulty;

			switch (difficulty)
			{
				case medium:	return	Constants::difficultyOptions[medium];
				case hard:		return	Constants::difficultyOptions[hard];
				default:		return	Constants::difficultyOptions[easy];
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
	}
}