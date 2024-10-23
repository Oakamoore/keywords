#ifndef KEYWORDS_UTIL_H
#define KEYWORDS_UTIL_H

#include "session.h"
#include <string>
#include <string_view>
#include <functional>

namespace Keywords
{
	namespace Util
	{
		std::string_view getStringFromDifficulty(SessionConfig::Difficulty difficulty);
		std::string convertToCase(std::string_view str, std::function<int(int)> conversion);
	}
}

#endif