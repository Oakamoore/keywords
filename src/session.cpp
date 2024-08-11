#include "session.h"
#include "random.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <sstream>
#include <type_traits>

namespace
{
	constexpr int g_canvasWidth {240};
	constexpr int g_canvasHeight {100};

	template <typename T>
	std::string toStringWithPrecision(T value, int precision)
	{
		std::ostringstream out {};
		out.precision(precision);

		// Prevent digit truncation
		out << std::fixed << value;

		return std::move(out).str();
	}

	template <typename T>
	ftxui::Element createStatElement(const std::string& statName, const T& value, char unit = '\0')
	{
		std::string str {std::is_floating_point_v<T> ? toStringWithPrecision(value, 2) : std::to_string(value)};

		return ftxui::hbox
		({
			ftxui::text(statName) | ftxui::bold,
			ftxui::text(str + unit) | ftxui::color(ftxui::Color::Cyan)
		 }) | ftxui::center;
	}
}

namespace Keywords
{
	Session::Session(/*SessionConfig config*/)
	{
	}

	ftxui::Element Session::draw()
	{
		ftxui::Canvas c {g_canvasWidth, g_canvasHeight};

		// Draw the words to the screen
		for (const auto& word : m_words)
			c.DrawText(word->getX(), word->getY(), word->getText(), word->getColor());

		constexpr auto inputBoxSize {g_canvasWidth / 6};
		constexpr auto statBoxSize {(g_canvasWidth / 2) - inputBoxSize};

		auto frame
		{
			ftxui::vbox
			({
				ftxui::text("KEYWORDS") | ftxui::center,
				ftxui::separatorEmpty(),
				ftxui::canvas(std::move(c)) | ftxui::border,
				ftxui::hbox
				({
					m_input.draw() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, inputBoxSize),
					ftxui::separatorEmpty(),
					ftxui::window
					(
						ftxui::text("Session Stats") | ftxui::color(ftxui::Color::GrayDark) | ftxui::center,
						ftxui::hbox
						({
							ftxui::filler(),
							createStatElement("Time: ", m_uptime.elapsed(), 's'), ftxui::filler(),
							createStatElement("Number of Words: ", m_words.size()), ftxui::filler(),
							createStatElement("Misses: ", m_misses), ftxui::filler(),
							createStatElement("Capacity: ", m_words.capacity()), ftxui::filler(),
						})
					) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, statBoxSize),
				}),
				 ftxui::separatorEmpty()
			}) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, g_canvasWidth / 2)
		};

		return frame;
	}

	void Session::update()
	{
		static double s_timeStamp {0.0};
		constexpr Timer::Second spawnDelay {2};

		// Continue while 'm_misses' < 'g_maxMisses' or a 'SessionConfig' defined max
		
		// Move the words across the screen
		for (auto& word : m_words)
			word->move(g_canvasWidth);

		// The session has just begun, or the delay between spawns has passed 
		if (s_timeStamp == 0.0 || (m_uptime.elapsed() - s_timeStamp) >= spawnDelay.count())
		{
			addWords();
			s_timeStamp = m_uptime.elapsed();
		}

		eraseWords();
	}

	void Session::addWords()
	{
		constexpr int spawnCount {3};

		for (int i {0}; i < spawnCount; ++i)
			m_words.emplace_back(std::make_unique<Word>("test", Random::get(0, g_canvasHeight - 1)));

		// Randomly reads in from 'WordBank', then creates new 'Word' object
		// Adds a given amount of 'Word' objects to 'm_words'
		// Randomly assigns height values, ensuring they are all different
			// Check that the position being drawn to is empty
		// Take into account the word offset starting positions
	}

	void Session::eraseWords()
	{
		// When 'ENTER' is pressed, loops through 'm_words' 
		// and checks against 'Word::m_content' if they match 
		// 'm_input::getContent()', they are erased

		// Erase off screen words
		std::erase_if(m_words, [&] (const auto& word)
		{
			if (word->getX() >= g_canvasWidth)
			{
				++m_misses;
				return true;
			}

			return false;
		});
	}
}