#include "game.h"
#include "session.h"
#include "constants.h"
#include "word_bank.h"
#include <ftxui/component/event.hpp>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <vector>
#include <memory>

namespace
{
	const std::filesystem::path g_wordList {"tests/resources/valid_words_file.txt"};

	/*class SessionTest
	{
	public:
		auto getWords(const Keywords::Session& session) const { return session.m_words; }
		int getMisses(const Keywords::Session& session) const { return session.m_misses; }
		void resetSpawnDelay(Keywords::Session& session) { session.m_timeStamp = 0.0; }
	};*/
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

	auto wordCount {session.getWords().size()};
	
	REQUIRE(session.getInputComponent().content.empty());

	SECTION("A correct word is typed")
	{
		auto firstWord {session.getWords().front()->text};

		// Fill the component with a valid word
		session.getInputComponent().content = firstWord;

		REQUIRE_FALSE(session.getInputComponent().content.empty());

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		session.update();

		// The 'ENTER' keypress should empty the input component
		REQUIRE(session.getInputComponent().content.empty());

		// The number of words should have decreased
		REQUIRE(session.getWords().size() < wordCount);
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
		REQUIRE(session.getWords().size() == wordCount);
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

//TEST_CASE("End a session when a given lose condition is met")
//{
//	bool hasLost {};
//	auto lose {[&] { hasLost = true; }};
//
//	Keywords::Session session {Keywords::SessionConfig {}, {}, nullptr, lose};
//	SessionTest sessionTest {};
//
//	auto component {Keywords::getSessionComponent(session)};
//
//	// The component should render without crashing
//	component->Render();
//
//	// Populate the word bank
//	Keywords::WordBank::readFromFile(g_wordList);
//
//	while (true)
//	{
//		// Spawn the first batch of words
//		session.update();
//
//		sessionTest.resetSpawnDelay(session); 
//
//		if (sessionTest.getMisses(session) == Keywords::Constants::maxMisses)
//		{
//			try
//			{
//				session.update();
//			}
//			catch (...) { }
//
//			break;
//		}
//	}
//}

//TEST_CASE("Insert a new entry in a given save file")
//{
//	SECTION("Pass in a file that doesn't exist")
//	{
//
//	}
//}