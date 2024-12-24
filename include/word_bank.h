#ifndef KEYWORDS_WORD_BANK_H
#define KEYWORDS_WORD_BANK_H

#include "game_config.h"
#include <array>
#include <vector>
#include <string>
#include <filesystem>

namespace Keywords
{
	class WordBank
	{
	public:
		WordBank(std::array<std::filesystem::path, GameConfig::max_difficulty> filePaths);

		const std::vector<std::string>& getEasyWords() const { return m_easyWords; }
		const std::vector<std::string>& getMediumWords() const { return m_mediumWords; }
		const std::vector<std::string>& getHardWords() const { return m_hardWords; }

	private:
		std::vector<std::string> m_easyWords {};
		std::vector<std::string> m_mediumWords {};
		std::vector<std::string> m_hardWords {};
	};
}

#endif 
