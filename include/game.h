#ifndef KEYWORDS_GAME_H
#define KEYWORDS_GAME_H

#include "session.h"
#include <ftxui/component/component.hpp>

namespace Keywords
{
	void startGame();

	ftxui::Component getSessionComponent(const Session& session);
	ftxui::Component getMainMenuComponent(SessionConfig& config, InputComponent& inputComponent);
}

#endif