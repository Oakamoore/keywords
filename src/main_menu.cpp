#include "main_menu.h"
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
		using namespace std::string_view_literals;
		
		static constexpr std::array difficultyOptions {"easy"sv, "medium"sv, "hard"sv};

		for (std::size_t i {0}; i < difficultyOptions.size(); ++i)
		{
			if (str == difficultyOptions[i])
				return static_cast<Keywords::SessionConfig::Difficulty>(i);
		}

		return {};
	}

	ftxui::Element getAdjacentText(const auto& ...e)
	{
		return ftxui::hbox({e...}) | ftxui::center;
	}

	ftxui::Element getColoredText(const std::string& str, const ftxui::Color& color = ftxui::Color::Yellow)
	{
		return ftxui::text(str) | ftxui::color(color) | ftxui::center;
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
		auto description
		{
			ftxui::vbox
			({
				ftxui::separatorEmpty(),
				getAdjacentText(ftxui::text("To begin a game session type "), getColoredText("play"), ftxui::text(".")),
				ftxui::separatorEmpty(),
				ftxui::paragraphAlignCenter("To change the difficulty of a game session (when none are active), type either:"),
				getAdjacentText(getColoredText("easy"), ftxui::text(" (<=5 character words)")),
				getAdjacentText(getColoredText("medium"), ftxui::text(" (>= 6 and <= 8 character words)")),
				getAdjacentText(getColoredText("hard"), ftxui::text(" (>=9 character words)")),
				ftxui::separatorEmpty(),
				ftxui::paragraphAlignCenter("During a game session, type the onscreen words."),
				ftxui::separatorEmpty(),
				getAdjacentText(ftxui::text("Press "), getColoredText("ENTER"), ftxui::text(" to confirm a typed word.")),
				getAdjacentText(ftxui::text("Press "), getColoredText("BACKSPACE"), ftxui::text(" to erase individual characters.")),
				getAdjacentText(ftxui::text("Press "), getColoredText("CTRL + W"), ftxui::text(" to erase an entire word all at once.")),
				getAdjacentText(ftxui::text("Press "), getColoredText("ESCAPE"), ftxui::text(" to quit the game at any time."))
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
	void MainMenu::handleInput(SessionConfig& config, InputComponent& inputComponent, const std::function<void()>& quit)
	{
		if (inputComponent.hasPressedEscape)
			quit();

		if (inputComponent.hasPressedEnter)
		{
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