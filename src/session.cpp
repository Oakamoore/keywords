#include "session.h"
#include "word.h"
#include "random.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <sstream>

namespace
{
	constexpr int g_canvasWidth {240};
	constexpr int g_canvasHeight {100};

	std::string toStringWithPrecision(double value, int precision)
	{
		std::ostringstream out {};

		// Set the max precision of the stream
		out.precision(precision);
		
		// Read the value into the stream
		// and prevent digit truncation 
		out << std::fixed << value;
		
		return std::move(out).str();
	}
}

namespace Keywords
{
	Session::Session(/*SessionConfig config*/)
	{
	}

	ftxui::Element Session::draw()
	{
		// Create a temporary blank 'ftxui::Canvas' 
		ftxui::Canvas c {g_canvasWidth, g_canvasHeight};

		for (const auto& word : m_words)
			c.DrawText(word->getX(), word->getY(), word->getText(), word->getColor());

		// TODO: Call 'Session::update()' post draw phase
		update();

		static constexpr auto inputBoxSize {g_canvasWidth / 6};
		static constexpr auto statusBoxSize {(g_canvasWidth / 2) - inputBoxSize};

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
							ftxui::hbox
							({
								ftxui::text("Time: ") | ftxui::bold,
								ftxui::text(toStringWithPrecision(m_uptime.elapsed(), 2))
							}) | ftxui::center, ftxui::filler(),
							ftxui::hbox
							({
								ftxui::text("Number of Words: ") | ftxui::bold,
								ftxui::text(std::to_string(m_words.size()))
							}) | ftxui::center, ftxui::filler(),
							ftxui::hbox
							({
								ftxui::text("Misses: ") | ftxui::bold,
								ftxui::text(std::to_string(m_misses))
							}) | ftxui::center, ftxui::filler(),
							ftxui::hbox
							({
								ftxui::text("Test: ") | ftxui::bold,
								ftxui::text(std::to_string(m_misses))
							}) | ftxui::center,
							ftxui::filler()
						})
					) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, statusBoxSize),
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