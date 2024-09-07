#include "word_bank.h"
#include <cctype>
#include <fstream>
#include <exception>
#include <algorithm>

namespace
{
	bool hasNonAlphaChars(std::string_view str)
	{
		return std::ranges::find_if(str, [] (unsigned char c) { return !std::isalpha(c); }) != str.end();
	}

	bool hasUppercaseChars(std::string_view str)
	{
		return std::ranges::find_if(str, [] (unsigned char c) { return std::isupper(c); }) != str.end();
	}

	std::string convertToLowercase(std::string_view str)
	{
		if (!str.empty())
		{
			// Construct a string of a given length
			std::string lowercase(str.length(), ' ');

			// Assign lowercase characters to the newly constructed string
			std::transform(str.begin(), str.end(), lowercase.begin(), [] (unsigned char c)
			{
				return static_cast<char>(std::tolower(c));
			});

			return lowercase;
		}

		return std::string {};
	}

	void clearWordBanks()
	{
		Keywords::WordBank::easyWords.clear();
		Keywords::WordBank::mediumWords.clear();
		Keywords::WordBank::hardWords.clear();
	}
}

namespace Keywords
{
	namespace WordBank
	{
		void readFromFile(const std::filesystem::path& filePath)
		{
			if (!std::filesystem::exists(filePath))
				throw std::runtime_error("Failed to locate file");

			if (std::filesystem::is_empty(filePath))
				throw std::runtime_error("Cannot read from an empty file");

			std::ifstream file {filePath.string()};

			if (!file.is_open())
				throw std::runtime_error("Failed to open file");

			auto isEasyWord {[](std::string_view str) { return str.length() <= 5; }};
			auto isMediumWord {[](std::string_view str) { return str.length() >= 6 && str.length() <= 8; }};

			// Ensures the word banks are empty
			// before populating them with words
			clearWordBanks();

			std::string word {};

			while (std::getline(file, word))
			{
				// Skip to the next word
				if (hasNonAlphaChars(word) || word.empty())
					continue;

				if (hasUppercaseChars(word))
					word = convertToLowercase(word);

				// Populate the appropriate word bank
				if (isEasyWord(word))
					easyWords.push_back(word);
				else if (isMediumWord(word))
					mediumWords.push_back(word);
				else
					hardWords.push_back(word);
			}

			if (easyWords.empty() || mediumWords.empty() || hardWords.empty())
				throw std::runtime_error("Failed to populate all word banks");

			file.close();
		}
	}
}