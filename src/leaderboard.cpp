#include "leaderboard.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace
{
	
}

namespace Keywords
{
	void Leaderboard::handleInput([[maybe_unused]] InputComponent& inputComponent)
	{
		// 'ENTER' and 'ESCAPE' both result in the component being quit


	}

	ftxui::Component getLeaderboard([[maybe_unused]] InputComponent& inputComponent)
	{
		return inputComponent.component | ftxui::center;
	}
}