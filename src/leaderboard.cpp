#include "leaderboard.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace
{
	
}

namespace Keywords
{
	void Leaderboard::handleInput(InputComponent& inputComponent, std::function<void()> quit,
								  std::function<void()> save)
	{
		if (inputComponent.hasPressedEscape)
			quit();



	}

	ftxui::Component getLeaderboard([[maybe_unused]] InputComponent& inputComponent)
	{
		return inputComponent.component | ftxui::center;
	}
}