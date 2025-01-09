#include "word_bank.h"
#include "game_config.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <filesystem>
#include <exception>
#include <array>

using Catch::Matchers::Message;

TEST_CASE("Pass in files that don't exist")
{
	// The appropriate exception should be thrown
	REQUIRE_THROWS_MATCHES(Keywords::WordBank {{}}, std::runtime_error, Message("Failed to locate word bank"));
}

TEST_CASE("Pass in empty files")
{
	std::array <std::filesystem::path, Keywords::GameConfig::max_difficulty> filePaths {};
	
	filePaths.fill("tests/resources/empty_file.txt");

	// The appropriate exception should be thrown
	REQUIRE_THROWS_MATCHES(Keywords::WordBank {filePaths}, std::runtime_error, Message("Cannot read from an empty word bank"));
}

TEST_CASE("Pass in valid files")
{
	std::array <std::filesystem::path, Keywords::GameConfig::max_difficulty> filePaths 
	{
		"tests/resources/easy_words_file.txt",
		"tests/resources/medium_words_file.txt",
		"tests/resources/hard_words_file.txt"
	};

	Keywords::WordBank wordBank {filePaths};

	REQUIRE(wordBank.getEasyWords().size() == 20);		// 20/60 should be 'easy'
	REQUIRE(wordBank.getMediumWords().size() == 20);	// 20/60 should be 'medium'
	REQUIRE(wordBank.getHardWords().size() == 20);		// 20/60 should be 'hard'
}