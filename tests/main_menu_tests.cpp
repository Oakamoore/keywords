#include "game.h"
#include "session.h"
#include "input_component.h"
#include "main_menu.h"
#include "constants.h"
#include <ftxui/component/event.hpp>
#include <catch2/catch_test_macros.hpp>
#include <array>
#include <string_view>

namespace
{
	std::size_t getIndex(Keywords::SessionConfig::Difficulty difficulty)
	{
		return static_cast<std::size_t>(difficulty);
	}
}

TEST_CASE("Empty the input component following 'ENTER' keypress")
{
	Keywords::SessionConfig config {};
	Keywords::InputComponent inputComponent {};

	auto component {Keywords::getMainMenuComponent(config, inputComponent)};

	// The component should render without crashing
	component->Render();

	using enum Keywords::SessionConfig::Difficulty;

	// The default difficulty should be easy
	REQUIRE(config.difficulty == easy);

	REQUIRE(inputComponent.content.empty());		// Should be empty

	// Fill the component with dummy text
	inputComponent.content = "lorem";

	REQUIRE_FALSE(inputComponent.content.empty());	// Should be populated

	component->OnEvent(ftxui::Event::Return);

	// Responds to the above event
	Keywords::MainMenu::handleInput(config, inputComponent, nullptr, nullptr);

	// An 'ENTER' keypress event empties the input component
	REQUIRE(inputComponent.content.empty());

	// The nonsense input should have no effect on the configuration
	REQUIRE(config.difficulty == easy);
}

TEST_CASE("Change the difficulty of a session")
{
	Keywords::SessionConfig config {}; 
	Keywords::InputComponent inputComponent {};

	using enum Keywords::SessionConfig::Difficulty;

	// The default difficulty should be "easy"
	REQUIRE(config.difficulty == easy);

	auto component {Keywords::getMainMenuComponent(config, inputComponent)};
	
	// The component should render without crashing
	component->Render();

	SECTION("Retain the same previous difficulty")
	{
		// Fill input component with "easy"
		inputComponent.content = Keywords::Constants::difficultyOptions[getIndex(easy)];

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		Keywords::MainMenu::handleInput(config, inputComponent, nullptr, nullptr);

		// The difficulty stays the same
		REQUIRE(config.difficulty == easy);
	}

	SECTION("Change difficulty back to easy")
	{
		config.difficulty = hard;

		// Fill input component with "easy"
		inputComponent.content = Keywords::Constants::difficultyOptions[getIndex(easy)];

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		Keywords::MainMenu::handleInput(config, inputComponent, nullptr, nullptr);

		// The difficulty should have reverted
		REQUIRE(config.difficulty == easy);
	}

	SECTION("Change difficulty to medium")
	{
		// Fill input component with "medium"
		inputComponent.content = Keywords::Constants::difficultyOptions[getIndex(medium)];

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		Keywords::MainMenu::handleInput(config, inputComponent, nullptr, nullptr);

		// The difficulty should change
		REQUIRE(config.difficulty == medium);
	}

	SECTION("Change difficulty to hard")
	{
		// Fill input component with "hard"
		inputComponent.content = Keywords::Constants::difficultyOptions[getIndex(hard)];

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		Keywords::MainMenu::handleInput(config, inputComponent, nullptr, nullptr);

		// The difficulty should change 
		REQUIRE(config.difficulty == hard);
	}
}

TEST_CASE("Allow quitting from the main menu")
{
	Keywords::SessionConfig config {};
	Keywords::InputComponent inputComponent {};

	auto component {Keywords::getMainMenuComponent(config, inputComponent)};

	// The component should render without crashing
	component->Render();

	bool hasQuit {};
	auto quit {[&] { hasQuit = true; }};

	REQUIRE(hasQuit == false);

	component->OnEvent(ftxui::Event::Escape);

	// Respond to the above event
	Keywords::MainMenu::handleInput(config, inputComponent, quit, nullptr);

	REQUIRE(hasQuit == true);
}

TEST_CASE("Allow a game session to be started")
{
	Keywords::SessionConfig config {};
	Keywords::InputComponent inputComponent {};

	auto component {Keywords::getMainMenuComponent(config, inputComponent)};

	// The component should render without crashing
	component->Render();

	bool isPlaying {};
	auto play {[&] { isPlaying = true; }};

	// Fill the input component with "play"
	inputComponent.content = Keywords::Constants::playInput;

	REQUIRE(isPlaying == false);

	component->OnEvent(ftxui::Event::Return);
	
	// Respond to the above event
	Keywords::MainMenu::handleInput(config, inputComponent, nullptr, play);

	REQUIRE(isPlaying == true);
}