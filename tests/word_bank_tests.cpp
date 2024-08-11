#include "word_bank.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <filesystem>
#include <exception>
#include <algorithm>

namespace
{
	bool hasUppercaseChars(std::string_view str)
	{
		return std::ranges::find_if(str, [] (unsigned char c) { return std::isupper(c); }) != str.end();
	}
}

using Catch::Matchers::Message;

TEST_CASE("Pass in a file that doesn't exist")
{
	const std::filesystem::path filePath {"tests/resources/file.txt"};

	// The appropriate exception should be thrown
	REQUIRE_THROWS_MATCHES(Keywords::WordBank::readFromFile(filePath), std::runtime_error, Message("Failed to locate file"));
}

TEST_CASE("Pass in an empty file")
{
	const std::filesystem::path filePath {"tests/resources/empty_file.txt"};

	// The appropriate exception should be thrown
	REQUIRE_THROWS_MATCHES(Keywords::WordBank::readFromFile(filePath), std::runtime_error, Message("Cannot read from an empty file"));
}

TEST_CASE("Pass in a file full of invalid words")
{
	const std::filesystem::path filePath {"tests/resources/invalid_words_file.txt"};

	// The appropriate exception should be thrown
	REQUIRE_THROWS_MATCHES(Keywords::WordBank::readFromFile(filePath), std::runtime_error, Message("Failed to populate all word banks"));

	// All three word banks should be empty
	REQUIRE(Keywords::WordBank::easyWords.empty());
	REQUIRE(Keywords::WordBank::mediumWords.empty());
	REQUIRE(Keywords::WordBank::hardWords.empty());
}

TEST_CASE("Pass in a file full of valid words")
{
	const std::filesystem::path filePath {"tests/resources/valid_words_file.txt"};

	Keywords::WordBank::readFromFile(filePath);

	// Separate the words into their appropriate category
	REQUIRE(Keywords::WordBank::easyWords.size() == 20);	// 20/60 should be 'easy'
	REQUIRE(Keywords::WordBank::mediumWords.size() == 20);	// 20/60 should be 'medium'
	REQUIRE(Keywords::WordBank::hardWords.size() == 20);	// 20/60 should be 'hard'
}

TEST_CASE("Pass in a file containing only 'easy' words")
{
	const std::filesystem::path filePath {"tests/resources/easy_words_file.txt"};

	// The appropriate exception should be thrown
	REQUIRE_THROWS_MATCHES(Keywords::WordBank::readFromFile(filePath), std::runtime_error, Message("Failed to populate all word banks"));

	// Only 'easyWords' should be populated
	REQUIRE(Keywords::WordBank::easyWords.size() == 20);	// 20/20 should be 'easy'
	REQUIRE(Keywords::WordBank::mediumWords.empty());		// 0/20 should be 'medium'
	REQUIRE(Keywords::WordBank::hardWords.empty());			// 0/20 should be 'hard'
}

TEST_CASE("Pass in a file containing only 'medium' words")
{
	const std::filesystem::path filePath {"tests/resources/medium_words_file.txt"};

	// The appropriate exception should be thrown
	REQUIRE_THROWS_MATCHES(Keywords::WordBank::readFromFile(filePath), std::runtime_error, Message("Failed to populate all word banks"));

	// Only 'mediumWords' should be populated
	REQUIRE(Keywords::WordBank::easyWords.empty());			// 0/20 should be 'easy'
	REQUIRE(Keywords::WordBank::mediumWords.size() == 20);  // 20/20 should be 'medium'
	REQUIRE(Keywords::WordBank::hardWords.empty());			// 0/20 should be 'hard'
}

TEST_CASE("Pass in a file containing only 'hard' words")
{
	const std::filesystem::path filePath {"tests/resources/hard_words_file.txt"};

	// The appropriate exception should be thrown
	REQUIRE_THROWS_MATCHES(Keywords::WordBank::readFromFile(filePath), std::runtime_error, Message("Failed to populate all word banks"));

	// Only 'hardWords' should be populated
	REQUIRE(Keywords::WordBank::easyWords.empty());			// 0/20 should be 'easy'
	REQUIRE(Keywords::WordBank::mediumWords.empty());		// 0/20 should be 'medium'
	REQUIRE(Keywords::WordBank::hardWords.size() == 20);	// 20/20 should be 'hard'
}

TEST_CASE("Pass in a file full of words containing uppercase characters")
{
	const std::filesystem::path filePath {"tests/resources/uppercase_words_file.txt"};

	// Should read in the words, and convert them to lowercase
	Keywords::WordBank::readFromFile(filePath);

	REQUIRE(Keywords::WordBank::easyWords.size() == 20);	// 20/60 words should be 'easy'

	// No words in 'easyWords' should contain uppercase characters
	for (const auto& word : Keywords::WordBank::easyWords)
		REQUIRE_FALSE(hasUppercaseChars(word));

	REQUIRE(Keywords::WordBank::mediumWords.size() == 20);	// 20/60 words should be 'medium'

	// No words in 'mediumWords' should contain uppercase characters
	for (const auto& word : Keywords::WordBank::mediumWords)
		REQUIRE_FALSE(hasUppercaseChars(word));

	REQUIRE(Keywords::WordBank::hardWords.size() == 20);	// 20/60 words should be 'hard'

	// No words in 'hardWords' should contain uppercase characters
	for (const auto& word : Keywords::WordBank::hardWords)
		REQUIRE_FALSE(hasUppercaseChars(word));
}