#ifndef KEYWORDS_MAIN_MENU_H
#define KEYWORDS_MAIN_MENU_H

#include "game_config.h"
#include "input_component.h"
#include <functional>

namespace Keywords
{
	namespace MainMenu
	{
		void handleInput(GameConfig& config, InputComponent& inputComponent,
						 std::function<void()> quit,
						 std::function<void()> play);
	}
}

#endif