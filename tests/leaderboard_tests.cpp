#include "game.h"
#include "leaderboard.h"
#include <ftxui/component/event.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <exception>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

namespace
{
	const std::filesystem::path g_saveFilePath {"tests/resources/leaderboard_entries_file.txt"};

	std::string getFileContents(const std::filesystem::path& filePath)
	{
		std::ifstream input {filePath};

		std::string line {};
		std::string contents {};

		while (std::getline(input, line))
			contents += line + '\n';

		return contents;
	}

	void resetFile(const std::filesystem::path& filePath)
	{
		// Mock entries to populate the file with
		std::vector<std::string> entries
		{
			{"2084|88|405|40|3.13|129.50|2024-11-06 23:14:12|-----\n"},
			{"682|51|223|35|2.62|85.20|2024-11-06 23:15:44|-----\n"},
			{"2108|104|471|42|3.18|148.01|2024-11-08 18:53:17|-----\n"},
			{"2108|104|471|42|3.18|148.08|2024-11-08 18:53:18|-----\n"},
			{"3071|127|566|45|3.39|167.01|2024-11-08 19:05:25|-----\n"}
		};
		
		std::ofstream output {filePath};

		// Write the above entries to the file 
		for (const auto& entry : entries)
			output << entry;
	}
}

TEST_CASE("Read session save data from a file")
{
	SECTION("Pass in valid file path")
	{
		REQUIRE_NOTHROW(Keywords::Leaderboard {{},{g_saveFilePath},{}});
	}

	SECTION("Pass in invalid file path")
	{
		REQUIRE_THROWS_MATCHES
		(
			(Keywords::Leaderboard {{}, {}, {}}),
			std::runtime_error,
			Catch::Matchers::Message("Unable to locate save file")
		);
	}
}

TEST_CASE("Append a username to a save file entry")
{
	Keywords::Leaderboard leaderboard {{}, g_saveFilePath, {}};

	auto component {Keywords::getLeaderboardComponent(leaderboard)};
	auto before {getFileContents(g_saveFilePath)};

	REQUIRE(leaderboard.getInputComponent().content.empty());

	SECTION("A valid username is entered")
	{
		// Fill the input component with a vaild username
		leaderboard.getInputComponent().content = "Username";

		REQUIRE_FALSE(leaderboard.getInputComponent().content.empty());

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		leaderboard.handleInput();

		auto after {getFileContents(g_saveFilePath)};

		// The contents of the file should change
		REQUIRE(before != after);

		resetFile(g_saveFilePath);
	}

	SECTION("An invalid username is entered")
	{
		// Fill the input component with an invalid username
		leaderboard.getInputComponent().content = "ThisUsernameIsTooLong";

		REQUIRE_FALSE(leaderboard.getInputComponent().content.empty());

		component->OnEvent(ftxui::Event::Return);

		// Respond to the above event
		leaderboard.handleInput();

		auto after {getFileContents(g_saveFilePath)};

		// The contents of the file should not change
		REQUIRE(before == after);
	}
}

TEST_CASE("Allow quitting from a leaderboard")
{
	bool hasQuit {};
	auto quit {[&] { hasQuit = true; }};

	Keywords::Leaderboard leaderboard {{}, {g_saveFilePath}, quit};

	auto component {Keywords::getLeaderboardComponent(leaderboard)};

	REQUIRE(hasQuit == false);

	component->OnEvent(ftxui::Event::Escape);

	// Respond to the above event
	leaderboard.handleInput();

	REQUIRE(hasQuit == true);
}