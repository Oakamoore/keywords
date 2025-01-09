#ifndef KEYWORDS_WORD_H
#define KEYWORDS_WORD_H

#include <ftxui/screen/color.hpp>
#include <string>
#include <string_view>

namespace Keywords
{
	struct Word
	{
		explicit Word(std::string_view text);

		void move();
		void updateColor(int cavasWidth);

		const std::string text {};
		int x {};
		int y {};
		ftxui::Color color {ftxui::Color::Green};
	};
}

#endif