#include "input_component.h"
#include <ftxui/component/event.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <cctype>

TEST_CASE("Disallow certain input events")
{
	Keywords::InputComponent inputComponent {};

	// The component should be drawn without crashing
	inputComponent.draw();

	SECTION("Uppercase characters should be ignored")
	{
		for (char c {'A'}; c <= 'Z'; ++c)
		{
			// Uppercase character events should be caught 
			REQUIRE(inputComponent.getComponent()->OnEvent(ftxui::Event::Character(c)) == true);

			// The input component should remain unchanged 
			REQUIRE(inputComponent.getContent().empty());
		}
	}

	SECTION("Digit characters should be ignored")
	{
		for (int i {0}; i <= 9; ++i)
		{
			// Digit character events should be caught
			REQUIRE(inputComponent.getComponent()->OnEvent(ftxui::Event::Character(std::to_string(i))) == true);

			// The input component should remain unchanged
			REQUIRE(inputComponent.getContent().empty());
		}
	}

	SECTION("Special characters should be ignored")
	{
		// Loop through the list of printable ASCII chars
		for (char c {' '}; c <= '~'; ++c)
		{
			// Not a letter, digit, or space character
			if (!std::isalpha(c) && !std::isdigit(c) && c != ' ')
			{
				// Special character events should be caught
				REQUIRE(inputComponent.getComponent()->OnEvent(ftxui::Event::Character(c)) == true);

				// The input component should remain unchanged
				REQUIRE(inputComponent.getContent().empty());
			}
		}
	}
}

TEST_CASE("Detect enter keypress")
{
	Keywords::InputComponent inputComponent {};

	// The component should be drawn without crashing
	inputComponent.draw();

	inputComponent.getComponent()->OnEvent(ftxui::Event::Return);

	// Following an 'ENTER' event, the enter key flag should be true
	REQUIRE(inputComponent.hasPressedEnter());

	inputComponent.getComponent()->OnEvent(ftxui::Event::Custom);

	// Following a non 'ENTER' event, the enter key flag should be false
	REQUIRE_FALSE(inputComponent.hasPressedEnter());
}

TEST_CASE("Respond appropriately to a word deletion event")
{
	Keywords::InputComponent inputComponent {};

	// The component should be drawn without crashing
	inputComponent.draw();

	// Event tied to word deletion
	const auto ctrlW {ftxui::Event::Special("\x17")};

	const std::string blankSpaces {"         "};

	SECTION("An empty input component")
	{
		inputComponent.getComponent()->OnEvent(ctrlW);

		// A deletion event shouldn't affect an empty input component
		REQUIRE(inputComponent.getContent().empty());
	}

	SECTION("An input component only containing blank spaces")
	{
		// Fill the component with blank spaces
		inputComponent.setContent(blankSpaces);

		inputComponent.getComponent()->OnEvent(ctrlW);

		// All of the blank spaces should be deleted
		REQUIRE(inputComponent.getContent().empty());
	}

	SECTION("An input component containing blank spaces followed by a word")
	{
		inputComponent.setContent(blankSpaces + "lorem");

		inputComponent.getComponent()->OnEvent(ctrlW);

		// The word should be deleted, leaving the blank spaces unchanged
		REQUIRE(inputComponent.getContent() == blankSpaces);
	}

	SECTION("An input component containing a word followed by blank spaces")
	{
		inputComponent.setContent("lorem" + blankSpaces);

		inputComponent.getComponent()->OnEvent(ctrlW);

		// The entire contents of the input component should be deleted
		REQUIRE(inputComponent.getContent().empty());
	}

	SECTION("An input component containing a single word")
	{
		inputComponent.setContent("lorem");

		inputComponent.getComponent()->OnEvent(ctrlW);

		// The word should be deleted, rendering the input component empty
		REQUIRE(inputComponent.getContent().empty());
	}

	SECTION("An input component containing several space separated words")
	{
		inputComponent.setContent("lorem ipsum dolor sit amet");

		// Two word deletion events
		inputComponent.getComponent()->OnEvent(ctrlW);
		inputComponent.getComponent()->OnEvent(ctrlW);

		// The last two words should be deleted, and the final space should be retained
		REQUIRE(inputComponent.getContent() == "lorem ipsum dolor ");
	}
}