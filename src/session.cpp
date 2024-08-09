#include "session.h"
#include "word.h"
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
	ftxui::Element getStatElement(const std::string& statName, const T& value)
	{
		std::string str {std::is_floating_point_v<T> ? toStringWithPrecision(value, 2) : std::to_string(value)};

		return ftxui::hbox
		({
			ftxui::text(statName) | ftxui::bold,
			ftxui::text(str) | ftxui::color(ftxui::Color::Cyan)
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
		// Create a blank 'ftxui::Canvas' 
		ftxui::Canvas c {g_canvasWidth, g_canvasHeight};

		for (const auto& word : m_words)
			c.DrawText(word->getX(), word->getY(), word->getText(), word->getColor());

		update();

		static constexpr auto inputBoxSize {g_canvasWidth / 6};
		static constexpr auto statBoxSize {(g_canvasWidth / 2) - inputBoxSize};

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
						ftxui::text("Session Stats") | ftxui::center,
						ftxui::hbox
						({
							ftxui::filler(),
							getStatElement("Time: ", m_uptime.elapsed()), ftxui::filler(),
							getStatElement("Number of Words: ", m_words.size()), ftxui::filler(),
							getStatElement("Misses: ", m_misses), ftxui::filler(),
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
		static auto timeStamp {m_uptime.elapsed()};
		static constexpr Timer::Second spawnDelay {2};

		// Continue while 'm_misses' < 'g_maxMisses' or a 'SessionConfig' defined max
		
		// Move the words across the screen
		for (auto& word : m_words)
			word->move(g_canvasWidth);

		// A given amount of time has passed 
		if ((m_uptime.elapsed() - timeStamp) >= spawnDelay.count())
		{
			addWords();
			timeStamp = m_uptime.elapsed();
		}

		eraseWords();
	}

	void Session::addWords()
	{
		static constexpr int spawnCount {3};

		for (int i {0}; i < spawnCount; ++i)
			m_words.emplace_back(std::make_unique<Word>("test", Random::get(10, g_canvasHeight - 1)));

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

	
	}
}