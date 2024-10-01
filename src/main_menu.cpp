#include "main_menu.h"
#include "constants.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <array>
#include <string>
#include <string_view>
#include <optional>
#include <functional>

namespace
{
	std::optional<Keywords::SessionConfig::Difficulty> getDifficultyFromString(std::string_view str)
	{
		using namespace Keywords;

		for (std::size_t i {0}; i < SessionConfig::difficultyCount; ++i)
		{
			if (str == Constants::difficultyOptions[i])
				return static_cast<SessionConfig::Difficulty>(i);
		}

		return {};
	}

	ftxui::Element getBorderLine()
	{
		constexpr int borderLineLength {85};

		// Return a separator of a given length
		return ftxui::vbox({ftxui::text(std::string(borderLineLength, ' ')), ftxui::separatorHeavy()}) | ftxui::center;
	}

	ftxui::Element getLogo()
	{
		auto logo
		{
			ftxui::vbox
			({
				ftxui::text(R"(    _______   _______   _______   _______   _______   _______   _______   _______    )"),
				ftxui::text(R"(   ||     || ||     || ||     || ||     || ||     || ||     || ||     || ||     ||   )"),
				ftxui::text(R"(   ||  K  || ||  E  || ||  Y  || ||  W  || ||  O  || ||  R  || ||  D  || ||  S  ||   )"),
				ftxui::text(R"(   ||_____|| ||_____|| ||_____|| ||_____|| ||_____|| ||_____|| ||_____|| ||_____||   )"),
				ftxui::text(R"(   |/_____\| |/_____\| |/_____\| |/_____\| |/_____\| |/_____\| |/_____\| |/_____\|   )"),
			})
		};

		return logo;
	}

	ftxui::Element getDescription()
	{
		using namespace ftxui;

		auto description
		{
			vbox
			({
				separatorEmpty(),
				hbox({text("To begin a game session type "), text("play") | color(Color::Yellow), text(".")}) | center,
				separatorEmpty(),
				paragraphAlignCenter("To change the difficulty of a game session (when none are active), type either:"),
				hbox({text("easy") | color(Color::Yellow), text(" (<=5 character words)")}) | center,
				hbox({text("medium") | color(Color::Yellow), text(" (>= 6 and <= 8 character words)")}) | center,
				hbox({text("hard") | color(Color::Yellow), text(" (>=9 character words)")}) | center,
				separatorEmpty(),
				hbox
				({
				 text("During a game session, type the onscreen words, and stay below "), 
				 text(std::to_string(Keywords::Constants::maxMisses) + " misses.") | color(Color::Red)
				}) | center,
				separatorEmpty(),
				hbox({text("Press "), text("ENTER") | color(Color::Yellow), text(" to confirm a typed word.")}) | center,
				hbox({text("Press "), text("BACKSPACE") | color(Color::Yellow), text(" to erase individual characters.")}) | center,
				hbox({text("Press "), text("CTRL + W") | color(Color::Yellow), text(" to erase an entire word all at once.")}) | center,
				hbox({text("Press "), text("ESCAPE") | color(Color::Yellow), text(" to quit the game at any time.")}) | center
			})
		};

		return description;
	}

	ftxui::Element getDifficultySelection(const Keywords::SessionConfig& config)
	{
		auto getRegularText {[](const std::string& str) { return ftxui::text(str) | ftxui::center; }};
		auto getInvertedText {[](const std::string& str) { return ftxui::text(str) | ftxui::inverted | ftxui::center; }};

		using enum Keywords::SessionConfig::Difficulty;

		auto easySelection {config.difficulty == easy ? getInvertedText(" easy ") : getRegularText(" easy ")};
		auto mediumSelection {config.difficulty == medium ? getInvertedText(" medium ") : getRegularText(" medium ")};
		auto hardSelection {config.difficulty == hard ? getInvertedText(" hard ") : getRegularText(" hard ")};

		ftxui::Element difficultySelection
		{
			ftxui::vbox
			({
				ftxui::separatorEmpty(),
				getRegularText("DIFFICULTY") | ftxui::bold | ftxui::underlined,
				ftxui::separatorEmpty(),
				easySelection, ftxui::separatorEmpty(),
				mediumSelection, ftxui::separatorEmpty(),
				hardSelection, ftxui::separatorEmpty(),
			}) | ftxui::center
		};

		return difficultySelection;
	}
}

namespace Keywords
{
	void MainMenu::handleInput(SessionConfig& config, InputComponent& inputComponent, 
							   std::function<void()> quit, 
							   std::function<void()> play)
	{
		if (inputComponent.hasPressedEscape)
			quit();

		if (inputComponent.hasPressedEnter)
		{
			if (inputComponent.content == Constants::playInput)
				play();
			
			auto difficulty {getDifficultyFromString(inputComponent.content)};

			if (difficulty)
				config.difficulty = *difficulty;

			// Clear the input component
			inputComponent.reset();
		}
	}

	ftxui::Component getMainMenuComponent(SessionConfig& config, InputComponent& inputComponent)
	{
		constexpr int inputBoxSize {25};

		auto component {ftxui::Renderer(inputComponent.component, [&]
		{
			return ftxui::vbox
			({
				ftxui::filler(),
				getLogo() | ftxui::center, ftxui::filler(),
				getBorderLine(), getDescription(), getBorderLine(), ftxui::filler(),
				getDifficultySelection(config), ftxui::filler(),
				inputComponent.draw() | ftxui::border
				| ftxui::size(ftxui::WIDTH, ftxui::EQUAL, inputBoxSize)
				| ftxui::notflex | ftxui::center,
				ftxui::filler(),
			}) | ftxui::border;
		})};
		
		return component;
	}
}