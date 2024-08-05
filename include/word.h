#ifndef KEYWORDS_WORD_H
#define KEYWORDS_WORD_H

#include <ftxui/screen/color.hpp>
#include <string>
#include <string_view>

namespace Keywords
{
	class Word
	{
	public:
		Word(std::string_view text, int canvasHeight);
		
		// Increments 'm_x' based on 's_speed'
		// Calls 'updateColor'
		void move(int canvasWidth);

		int getX() { return m_x; }
		int getY() { return m_y; }

		const std::string& getText() { return m_text; }
		const ftxui::Color& getColor() { return m_color; }

	private:
		// All 'Word' objects move at the same speed
		// though they have different starting positions 
		// that depend on the length of 'm_text'
		// This also prevents words from overlapping
		static constexpr int s_speed {1};

		// Called in 'Word::move()', takes in the max width of the canvas
		// and compares it to 'm_x' to determine the color
		void updateColor(int cavasWidth);

	private:
		std::string m_text {};
		int m_x {};
		int m_y {};
		ftxui::Color m_color {};

	};
}

#endif