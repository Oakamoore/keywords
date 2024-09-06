#include "main_menu.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <array>
#include <string_view>
#include <optional>

namespace
{
	using namespace std::string_view_literals;

	constexpr std::array g_difficultyOptions
	{
		"easy"sv,
		"medium"sv,
		"hard"sv
	};

	std::optional<Keywords::SessionConfig::Difficulty> getDifficultyFromString(std::string_view str)
	{
		for (std::size_t i {0}; i < g_difficultyOptions.size(); ++i)
		{
			if (str == g_difficultyOptions[i])
				return static_cast<Keywords::SessionConfig::Difficulty>(i);
		}

		return {};
	}

	ftxui::Element getDescription()
	{
		auto description
		{
			ftxui::vbox
			({
				ftxui::filler(), 
				ftxui::paragraphAlignCenter("To begin a game session type \"play\""),
				ftxui::paragraphAlignCenter("then press ENTER."),
				ftxui::separatorEmpty(),
				ftxui::separatorEmpty(),
				ftxui::paragraphAlignCenter("To change difficulty type either"),
				ftxui::paragraphAlignCenter("\"easy\", \"medium\" or \"hard\""),
				ftxui::paragraphAlignCenter("then press ENTER."),
				ftxui::filler(),
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
				getRegularText("DIFFICULTY") | ftxui::bold | ftxui::underlined,
				ftxui::separatorEmpty(),
				easySelection,
				ftxui::separatorEmpty(),
				mediumSelection,
				ftxui::separatorEmpty(),
				hardSelection,
				ftxui::separatorEmpty(),
				}) | ftxui::center
		};

		return difficultySelection;
	}
}

namespace Keywords
{
	void MainMenu::handleInput(SessionConfig& config, InputComponent& inputComponent)
	{
		if (inputComponent.hasPressedEnter)
		{
			auto difficulty {getDifficultyFromString(inputComponent.content)};

			if (difficulty)
				config.difficulty = *difficulty;

			// Clear the input component
			inputComponent.reset();
		}
	}

	ftxui::Component getMainMenuComponent(SessionConfig& config, 
										  InputComponent& inputComponent)
	{
		auto component {ftxui::Renderer(inputComponent.component, [&]
		{
			return
				ftxui::vbox({
				ftxui::filler(),
				ftxui::text("KEYWORDS") | ftxui::center,
				ftxui::filler(),
				getDescription(),
				ftxui::filler(),
				getDifficultySelection(config),
				ftxui::filler(),
				inputComponent.draw() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 25) | ftxui::notflex | ftxui::center,
				ftxui::filler(),
							}) | ftxui::border;
		})};
		
		return component;
	}
}