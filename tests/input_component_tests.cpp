#include "input_component.h"
#include "constants.h"
#include <ftxui/component/event.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <cctype>
#include <array>

TEST_CASE("Disallow/Allow certain input events")
{
	enum : std::size_t {filtered, unfiltered, max_input_types};

	std::array<Keywords::InputComponent, max_input_types> inputComponents
	{
		Keywords::InputComponent {},			// Filtered
		Keywords::InputComponent {{}, false}	// Unfiltered
	};

	// The components should be drawn without crashing
	inputComponents[filtered].draw();
	inputComponents[unfiltered].draw();

	SECTION("Uppercase characters should be ignored")
	{
		for (char c {'A'}; c <= 'Z'; ++c)
		{
			// Uppercase character events should be caught 
			REQUIRE(inputComponents[filtered].component->OnEvent(ftxui::Event::Character(c)) == true);

			// The input component should remain unchanged 
			REQUIRE(inputComponents[filtered].content.empty());
		}
	}

	SECTION("Digit characters should be ignored")
	{
		for (int i {0}; i <= 9; ++i)
		{
			// Digit character events should be caught
			REQUIRE(inputComponents[filtered].component->OnEvent(ftxui::Event::Character(std::to_string(i))) == true);

			// The input component should remain unchanged
			REQUIRE(inputComponents[filtered].content.empty());
		}
	}

	SECTION("Special characters should be ignored")
	{
		// Loop through the list of printable ASCII chars
		for (char c {' '}; c <= '~'; ++c)
		{
			// Ignore letters and digits 
			if (!std::isalpha(c) && !std::isdigit(c))
			{
				// Special character events should be caught
				REQUIRE(inputComponents[filtered].component->OnEvent(ftxui::Event::Character(c)) == true);

				// The input component should remain unchanged
				REQUIRE(inputComponents[filtered].content.empty());
			}
		}
	}

	SECTION("No characters should be ignored")
	{
		for (char c {' '}; c <= '~'; ++c)
		{
			// All character events should be caught
			REQUIRE(inputComponents[unfiltered].component->OnEvent(ftxui::Event::Character(c)) == true);

			// The input component should be populated
			REQUIRE_FALSE(inputComponents[unfiltered].content.empty());

			// Clear the component
			inputComponents[unfiltered].reset();
		}
	}
}

TEST_CASE("Detect given keypresses")
{
	Keywords::InputComponent inputComponent {};

	// The component should be drawn without crashing
	inputComponent.draw();

	SECTION("Detect enter keypress")
	{
		inputComponent.component->OnEvent(ftxui::Event::Return);

		// Following an 'ENTER' event, the enter key flag should be true
		REQUIRE(inputComponent.hasPressedEnter);

		inputComponent.component->OnEvent(ftxui::Event::Custom);

		// Following a non 'ENTER' event, the enter key flag should be false
		REQUIRE_FALSE(inputComponent.hasPressedEnter);
	}

	SECTION("Detect escape keypress")
	{
		inputComponent.component->OnEvent(ftxui::Event::Escape);

		// Following an 'ESCAPE' event, the escape key flag should be true
		REQUIRE(inputComponent.hasPressedEscape);

		inputComponent.component->OnEvent(ftxui::Event::Custom);

		// Following an non 'ESCAPE' event, the escape key flag should be false
		REQUIRE_FALSE(inputComponent.hasPressedEscape);
	}
}

TEST_CASE("Respond appropriately to a word deletion event")
{
	Keywords::InputComponent inputComponent {};

	// The component should be drawn without crashing
	inputComponent.draw();

	SECTION("An empty input component")
	{
		inputComponent.component->OnEvent(Keywords::Constants::wordDeletionEvent);

		// A deletion event shouldn't affect an empty input component
		REQUIRE(inputComponent.content.empty());
	}

	SECTION("An input component containing a single word")
	{
		inputComponent.content = "lorem";

		inputComponent.component->OnEvent(Keywords::Constants::wordDeletionEvent);

		// The word should be deleted, rendering the input component empty
		REQUIRE(inputComponent.content.empty());
	}
}