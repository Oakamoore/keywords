#include "word_bank.h"
#include "util.h"
#include <cctype>
#include <fstream>
#include <exception>
#include <algorithm>

namespace
{
	void fillWordBank(std::vector<std::string>& words, const std::filesystem::path& filePath)
	{
		std::ifstream file {filePath};

		if (!file.is_open())
			throw std::runtime_error("Failed to open word bank");

		std::string word {};

		while (std::getline(file, word))
			words.push_back(std::move(word));

		if (words.empty())
			throw std::runtime_error("Failed to populate word bank");

		file.close();
	}
}

namespace Keywords
{
	WordBank::WordBank(std::array<std::filesystem::path, GameConfig::max_difficulty> filePaths)
	{
		for (const auto& filePath : filePaths)
		{
			if (!std::filesystem::exists(filePath))
				throw std::runtime_error("Failed to locate word bank");

			if (std::filesystem::is_empty(filePath))
				throw std::runtime_error("Cannot read from an empty word bank");
		}

		using enum GameConfig::Difficulty;

		fillWordBank(m_easyWords, filePaths[easy]);
		fillWordBank(m_mediumWords, filePaths[medium]);
		fillWordBank(m_hardWords, filePaths[hard]);
	}
}