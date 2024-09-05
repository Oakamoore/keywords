#include "session.h"
#include "input_component.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <array>
#include <string_view>
#include <optional>

namespace
{
	constexpr std::size_t getDifficultyCount()
	{
		return static_cast<std::size_t>(Keywords::SessionConfig::Difficulty::max_difficulty);
	}

	using namespace std::string_view_literals;

	constexpr std::array<std::string_view, getDifficultyCount()> g_difficultyStrings
	{
		"easy"sv,
		"medium"sv,
		"hard"sv
	};

	std::optional<Keywords::SessionConfig::Difficulty> getDifficultyFromString(std::string_view str)
	{
		for (std::size_t i {0}; i < getDifficultyCount(); ++i)
		{
			if (str == g_difficultyStrings[i])
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
				ftxui::paragraphAlignCenter("To change difficulty type either"),
				ftxui::paragraphAlignCenter("\"easy\", \"medium\" or \"hard\""),
				ftxui::paragraphAlignCenter("then press ENTER."),
				ftxui::separatorEmpty(),
				ftxui::paragraphAlignCenter("To begin a game session type \"play\""),
				ftxui::paragraphAlignCenter("then press ENTER."),
				ftxui::filler(),
			})
		};

		return description;
	}

	ftxui::Element getDifficultySelection([[maybe_unused]] const Keywords::SessionConfig& config)
	{
		auto difficulty
		{
			ftxui::vbox
			({
				ftxui::text(" easy ") | ftxui::inverted | ftxui::center,
				ftxui::separatorEmpty(),
				ftxui::text(" medium ") | ftxui::inverted | ftxui::center,
				ftxui::separatorEmpty(),
				ftxui::text(" hard ") | ftxui::inverted | ftxui::center,
				ftxui::separatorEmpty(),
			 }) | ftxui::center
		};
		
		return difficulty;
	}
}

namespace Keywords
{
	ftxui::Component getMainMenuComponent([[maybe_unused]] SessionConfig& config, InputComponent& input)
	{
		auto component {ftxui::Renderer(input.component, [&]
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
				input.draw() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 25) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 5) | ftxui::notflex | ftxui::center,
				ftxui::filler(),
							}) | ftxui::border;
		})};

		return component;
	}
}