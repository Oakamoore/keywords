#include "session.h"
#include "word.h"
#include "random.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <array>

namespace
{
	constexpr int g_canvasWidth {240};
	constexpr int g_canvasHeight {100};
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

		c.DrawText(0, 0, "Time: " + std::to_string(m_uptime.elapsed()), ftxui::Color::White);
		c.DrawText(0, 5, "Number of Words: " +std::to_string(m_words.size()), ftxui::Color::White);

		// TODO: Loop through 'm_words' and draw each to the 'ftxui::Canvas'
		for (const auto& word : m_words)
			c.DrawText(word->getX(), word->getY(), word->getText(), word->getColor());

		// TODO: Call 'Session::update()' post draw phase
		update();

		return ftxui::vbox({
			ftxui::text("KEYWORDS") | ftxui::center, 
			ftxui::separatorEmpty(),
			ftxui::canvas(std::move(c)) | ftxui::border,
			m_input.draw() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, g_canvasWidth / 5),
			ftxui::separatorEmpty()
						   });
	}

	void Session::update()
	{
		static auto timeStamp {m_uptime.elapsed()};
		static constexpr Timer::Second spawnDelay {5};

		// Continue while 'm_misses' < 'g_maxMisses' or a 'SessionConfig' defined max
		

		// Loops through 'm_words', and increments their positions
		for (auto& word : m_words)
			word->move(g_canvasWidth);

		// Calls 'addWords()' and 'eraseWords()' 
		
		// A given amount of time has passed 
		if ((m_uptime.elapsed() - timeStamp) >= spawnDelay.count())
		{
			addWords();
			timeStamp = m_uptime.elapsed();
		}
	}

	void Session::addWords()
	{
		static constexpr int spawnCount {5};

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

		// Also removes any 'Word' that is out of bounds, incrementing 'm_misses'
		// 'Word::getX() >= g_canvasWidth'



	}
}