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
		static const auto& getWords(const Session& session) { return session.m_words; }
		static int& getMisses(Session& session) { return session.m_misses; }
		static double getSpawnDelay(const Session& session) { return session.getSpawnDelay(); }
		static void addWords(Session& session) { session.addWords(); }
		static void handleInput(Session& session) { session.handleInput(); }
	};
}

const std::filesystem::path g_wordList {"tests/resources/valid_words_file.txt"};

TEST_CASE("Respond appropriately to a typed word")
{
	Keywords::Session session {{}, {}, nullptr, nullptr};
	
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

	Keywords::Session session {{}, {}, back, nullptr};

	auto component {Keywords::getSessionComponent(session)};

	REQUIRE(isPlaying == true);

	component->OnEvent(ftxui::Event::Escape);

	// Respond to the above event
	session.update();

	REQUIRE(isPlaying == false);
}

TEST_CASE("Gradually decrease word spawn delay")
{
	Keywords::Session session {{}, {}, nullptr, nullptr};

	auto delay {Keywords::SessionTest::getSpawnDelay(session)};
	auto component {Keywords::getSessionComponent(session)};

	int iterations {10};

	while (iterations--)
	{
		// Spawn a batch of words
		Keywords::SessionTest::addWords(session);

		// Type out the spawned words
		for (const auto& word : Keywords::SessionTest::getWords(session))
		{
			if(word)
				session.getInputComponent().content = word->text;

			component->OnEvent(ftxui::Event::Return);

			Keywords::SessionTest::handleInput(session);
		}
	}

	// The delay should decrease after a certain amount of words are typed
	REQUIRE(delay > Keywords::SessionTest::getSpawnDelay(session));
}

TEST_CASE("End a session when a given lose condition is met")
{
	Keywords::WordBank::readFromFile(g_wordList);

	bool hasLost {};
	auto lose {[&] { hasLost = true; }};

	Keywords::Session session {{}, {}, nullptr, lose};

	// Meet the lose condition
	Keywords::SessionTest::getMisses(session) = Keywords::Constants::maxMisses;

	REQUIRE_THROWS_MATCHES(session.update(), std::runtime_error, Catch::Matchers::Message("Unable to locate save file"));

	REQUIRE(hasLost == true);
}

TEST_CASE("Insert a new entry in a given save file")
{
	Keywords::WordBank::readFromFile(g_wordList);

	const std::filesystem::path saveFilePath {"tests/resources/session_save_file.txt"};

	// Erase the contents of the file 
	std::ofstream file {saveFilePath};

	REQUIRE(std::filesystem::is_empty(saveFilePath));

	// Prevents 'std::bad_function_call' from being thrown
	auto emptyFunction {[] { ; }};

	Keywords::Session session {{}, saveFilePath, nullptr, emptyFunction};

	// Meet the lose condition
	Keywords::SessionTest::getMisses(session) = Keywords::Constants::maxMisses;

	// Writes to the save file 
	session.update();

	// The file has been populated 
	REQUIRE_FALSE(std::filesystem::is_empty(saveFilePath));
}