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

		GameConfig(bool isAudioEnabled = true)
			: isAudioEnabled {isAudioEnabled}
		{}

		Difficulty difficulty {};
		const bool isAudioEnabled {};
	};
}

#endif