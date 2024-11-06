#include "game.h"
#include "session.h"
#include "constants.h"
#include "word_bank.h"
#include <ftxui/component/event.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <filesystem>
#include <fstream>
#include <exception>

namespace Keywords
{
	struct SessionTest
	{
		static const auto& getWords(const Keywords::Session& session) { return session.m_words; }
		static int getMisses(const Keywords::Session& session) { return session.m_misses; }
	};
}

namespace
{
	const std::filesystem::path g_wordList {"tests/resources/valid_words_file.txt"};

	void runSession(Keywords::Session& session)
	{
		// Wait for the session to end
		while (Keywords::SessionTest::getMisses(session) != Keywords::Constants::maxMisses)
		{
			// Spawn a batch of words
			session.update();
		}
	}
}

TEST_CASE("Respond appropriately to a typed word")
{
	Keywords::Session session {Keywords::SessionConfig {}, {}, nullptr, nullptr};
	
	auto component {Keywords::getSessionComponent(session)};

	// The component should render without crashing
	component->Render();

	// Populate the word bank
	Keywords::WordBank::readFromFile(g_wordList);

	// Spawn the first batch of words
	session.update();

	auto wordCount {Keywords::SessionTest::getWords(session).size()};
	
	REQUIRE(session.getInputComponent().content.empty());

	SECTION("A correct word is typed")
	{
		auto firstWord {Keywords::SessionTest::getWords(session).front()->text};

		// Fill the component with a valid word
		session.getInputComponent().content = firstWord;

		REQUIRE_FALSE(session.getInputComponent().content.empty());

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		session.update();

		// The 'ENTER' keypress should empty the input component
		REQUIRE(session.getInputComponent().content.empty());

		// The number of words should have decreased
		REQUIRE(Keywords::SessionTest::getWords(session).size() < wordCount);
	}

	SECTION("An incorrect word is typed")
	{
		// Fill the component with invalid content
		session.getInputComponent().content = "INVALID";

		REQUIRE_FALSE(session.getInputComponent().content.empty());

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		session.update();

		// The 'ENTER' keypress should empty the input component
		REQUIRE(session.getInputComponent().content.empty());

		// The number of words should stay the same
		REQUIRE(Keywords::SessionTest::getWords(session).size() == wordCount);
	}
}

TEST_CASE("Allow quitting from a session")
{
	bool isPlaying {true};
	auto back {[&] { isPlaying = false; }};

	Keywords::Session session {Keywords::SessionConfig {}, {}, back, nullptr};

	auto component {Keywords::getSessionComponent(session)};

	REQUIRE(isPlaying == true);

	component->OnEvent(ftxui::Event::Escape);

	// Respond to the above event
	session.update();

	REQUIRE(isPlaying == false);
}

TEST_CASE("End a session when a given lose condition is met", "[.realtime]")
{
	bool hasLost {};
	auto lose {[&] { hasLost = true; }};

	Keywords::Session session {Keywords::SessionConfig {}, {}, nullptr, lose};

	// Populate the word bank
	Keywords::WordBank::readFromFile(g_wordList);

	runSession(session);

	REQUIRE_THROWS_MATCHES(session.update(), std::runtime_error, Catch::Matchers::Message("Unable to locate save file"));

	REQUIRE(hasLost == true);
}

TEST_CASE("Insert a new entry in a given save file", "[.realtime]")
{
	// Populate the word bank
	Keywords::WordBank::readFromFile(g_wordList);

	// Prevents 'std::bad_function_call' from being thrown
	auto emptyFunction {[] { ; }};

	const std::filesystem::path saveFilePath {"tests/resources/session_save_file.txt"};

	// Erase the contents of the file 
	std::ofstream file {saveFilePath};

	REQUIRE(std::filesystem::is_empty(saveFilePath));

	Keywords::Session session {Keywords::SessionConfig {}, saveFilePath, nullptr, emptyFunction};

	runSession(session);

	// Writes to the save file 
	session.update();

	// The file has been populated 
	REQUIRE_FALSE(std::filesystem::is_empty(saveFilePath));
}