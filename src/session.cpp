#include "session.h"
#include "random.h"
#include "word_bank.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <sstream>
#include <type_traits>

namespace
{
	constexpr int g_canvasWidth {240};
	constexpr int g_canvasHeight {100};
	constexpr int g_canvasCellWidth {2};
	constexpr int g_canvasCellHeight {4};

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

	std::vector<int> getCanvasRows()
	{
		std::vector<int> canvasRows {};

		for (int i {0}; i < g_canvasHeight; i += g_canvasCellHeight)
			canvasRows.push_back(i);

		return canvasRows;
	}

	int getMinStartPos(std::string_view str)
	{
		// Minimum horizontal starting position for a word to appear off screen
		return -static_cast<int>(str.length() * g_canvasCellWidth);
	}
}

namespace Keywords
{
	Session::Session(const SessionConfig& config)
		: m_config {config}
	{
		using enum SessionConfig::Difficulty;

		// Determine what word bank to pull words from
		switch (m_config.difficulty)
		{
			case medium:
				m_wordBank = &WordBank::mediumWords;
				break;
			case hard:
				m_wordBank = &WordBank::hardWords;
				break;
			default:
				m_wordBank = &WordBank::easyWords; 
				break;
		}
	}

	ftxui::Element Session::draw()
	{
		// Create a blank canvas
		ftxui::Canvas c {g_canvasWidth, g_canvasHeight};

		// Draw the words to the canvas
		for (const auto& word : m_words)
			c.DrawText(word->getX(), word->getY(), word->getText(), word->getColor());

		constexpr auto inputBoxSize {g_canvasWidth / 6};
		constexpr auto statBoxSize {(g_canvasWidth / g_canvasCellWidth) - inputBoxSize};

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
			}) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, g_canvasWidth / g_canvasCellWidth)
		};

		return frame;
	}

	void Session::update()
	{
		static double s_timeStamp {0.0};
		constexpr Timer::Second spawnDelay {3};

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

	bool Session::isWordPresent(std::string_view str) const
	{
		for (const auto& word : m_words)
		{
			if (word->getText() == str)
				return true;
		}
		
		return false;
	}

	bool Session::isWordOverlapping(const Word& newWord) const
	{
		if (m_words.empty())
			return false;

		// The final cell a given word occupies
		const auto newWordEndPos {newWord.getX() + newWord.getText().length() * g_canvasCellWidth};

		for (const auto& currentWord : m_words)
		{
			// Check against words that have 
			// yet to appear on screen
			if (currentWord->getX() < 0)
			{
				// The final cell the current word occupies
				const auto currentWordEndPos {currentWord->getX() + currentWord->getText().length() * g_canvasCellWidth};

				bool isInSameRow {currentWord->getY() == newWord.getY()};

				bool isOverlappingInX
				{
					newWord.getX() <= currentWord->getX() && newWordEndPos >= currentWord->getX() || 
					newWord.getX() >= currentWord->getX() && (newWordEndPos <= currentWordEndPos || newWordEndPos >= currentWordEndPos)
				};

				if (isInSameRow && isOverlappingInX)
					return true;
			}
		}

		return false;
	}

	bool Session::isEveryWordVisible() const
	{
		// A word in the previous batch of spawned 
		// words has yet to appear on screen
		for (const auto& word : m_words)
		{
			if (word->getX() < 0)
				return true;
		}

		return false;
	}

	Word Session::getRandomWord() const
	{
		std::string str {};

		// Ensure that on screen words are different
		while (str.empty() || isWordPresent(str))
			str = Random::getElement(*m_wordBank);

		Word word {str, 0, 0};

		constexpr int minOffset {10};
		constexpr int maxOffset {100};

		static const std::vector<int> s_canvasRows {std::move(getCanvasRows())};

		// Prevent infinite overlap check
		int numAttempts {100};

		do
		{
			int xPos {getMinStartPos(str) - Random::get(minOffset, maxOffset)};
			int yPos {Random::getElement(s_canvasRows)};

			// Reposition the word
			word.setX(xPos);
			word.setY(yPos);

		} while (isWordOverlapping(word) && numAttempts--);

		return word;
	}


	void Session::addWords()
	{
		constexpr int spawnCount {100};

		if (!isEveryWordVisible())
		{
			for (int i {0}; i < spawnCount; ++i)
			{
				m_words.push_back(std::make_unique<Word>(std::move(getRandomWord())));
			}
		}
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