#include "word_bank.h"
#include <iostream>
#include <filesystem>

int main()
{
	
	std::filesystem::path filePath {"resources/us-english-language-dictionary.txt"};

	try
	{
		Keywords::WordBank::readFromFile(filePath);

		std::cout << '\n';
		std::cout << "Easy: " << Keywords::WordBank::easyWords.size() << '\n';
		std::cout << "Medium: " << Keywords::WordBank::mediumWords.size() << '\n';
		std::cout << "Hard: " << Keywords::WordBank::hardWords.size() << '\n';
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what();
	}

	return 0;
}