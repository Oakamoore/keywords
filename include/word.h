#ifndef KEYWORDS_WORD_H
#define KEYWORDS_WORD_H

#include <ftxui/screen/color.hpp>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace Keywords
{
	class Word
	{
	public:
		Word(std::string_view text);
		
		void move();
		void updateColor(int cavasWidth);

		void setX(int x) { m_x = x; }
		int getX() const { return m_x; }
		
		void setY(int y) { m_y = y; }
		int getY() const { return m_y; }

		const std::string& getText() const { return m_text; }
		const ftxui::Color& getColor() const { return m_color; }

	private:
		static constexpr int s_speed {1};

	private:
		std::string m_text {};
		int m_x {};
		int m_y {};
		ftxui::Color m_color {};

	};

	using Words = std::vector<std::unique_ptr<Word>>;
}

#endif