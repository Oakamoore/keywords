#ifndef KEYWORDS_WORD_BANK_H
#define KEYWORDS_WORD_BANK_H

#include <vector>
#include <string>
#include <filesystem>

namespace Keywords
{
	namespace WordBank
	{
		inline std::vector<std::string> easyWords {};
		inline std::vector<std::string> mediumWords {};
		inline std::vector<std::string> hardWords {};

		void readFromFile(const std::filesystem::path& filePath);
	}
}

#endif 
