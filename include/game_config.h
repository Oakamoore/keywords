#ifndef KEYWORDS_GAME_CONFIG_H
#define KEYWORDS_GAME_CONFIG_H

#include <cstddef>

namespace Keywords
{
	struct GameConfig
	{
		enum Difficulty : std::size_t
		{
			easy,
			medium,
			hard,
			max_difficulty
		};

		Difficulty difficulty {};
	};
}

#endif