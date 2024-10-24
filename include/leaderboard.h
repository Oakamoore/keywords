#ifndef KEYWORDS_LEADERBOARD_H
#define KEYWORDS_LEADERBOARD_H

#include "input_component.h"
#include <functional>
#include <filesystem>

namespace Keywords
{
	namespace Leaderboard
	{
		void handleInput(InputComponent& inputComponent, const std::filesystem::path& saveFilePath,
						 std::function<void()> quit, std::function<void()> save);
	}
}

#endif