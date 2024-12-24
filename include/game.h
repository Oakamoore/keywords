#ifndef KEYWORDS_GAME_H
#define KEYWORDS_GAME_H

#include "game_config.h"
#include "session.h"
#include "leaderboard.h"
#include <ftxui/component/component.hpp>

namespace Keywords
{
	void startGame();

	ftxui::Component getMainMenuComponent(GameConfig& config, InputComponent& inputComponent);
	ftxui::Component getSessionComponent(Session& session);
	ftxui::Component getLeaderboardComponent(Leaderboard& leaderboard);
}

#endif