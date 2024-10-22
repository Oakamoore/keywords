#ifndef KEYWORDS_LEADERBOARD_H
#define KEYWORDS_LEADERBOARD_H

#include "input_component.h"
#include <functional>

namespace Keywords
{
	namespace Leaderboard
	{
		void handleInput(InputComponent& inputComponent, std::function<void()> quit,
						 std::function<void()> save);
	}
}

#endif