#ifndef KEYWORDS_MAIN_MENU_H
#define KEYWORDS_MAIN_MENU_H

#include "input_component.h"
#include "session.h"
#include <functional>

namespace Keywords
{
	namespace MainMenu
	{
		void handleInput(SessionConfig& config, 
						 InputComponent& inputComponent, 
						 const std::function<void()>& quit);
	}
}

#endif