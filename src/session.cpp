#include "session.h"
#include "random.h"
#include "word_bank.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <sstream>
#include <type_traits>
#include <cmath>

namespace
{
	constexpr int g_canvasWidth {240};
	constexpr int g_canvasHeight {100};
	constexpr int g_canvasCellWidth {2};
	constexpr int g_canvasCellHeight {4};

	std::string toStringWithPrecision(const auto& value, int precision)
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

	int getWordStartPosition(std::string_view str)
	{
		// Minimum horizontal starting position for a word to appear off screen
		return -static_cast<int>(str.length() * g_canvasCellWidth);
	}

	int getWordEndPosition(const Keywords::Word& word)
	{
		// The canvas cell occupied by the last letter of a given word
		return static_cast<int>(word.x + word.text.length() * g_canvasCellWidth);
	}

	bool isWithinRange(int valueOne, int valueTwo, int deviation)
	{
		return std::abs(valueOne - valueTwo) <= deviation;
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
			c.DrawText(word->x, word->y, word->text, word->color);

		constexpr int inputBoxSize {g_canvasWidth / 6};
		constexpr int statBoxSize {(g_canvasWidth / g_canvasCellWidth) - inputBoxSize};

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
		
		// Update word position and color
		for (auto& word : m_words)
		{
			word->move();
			word->updateColor(g_canvasWidth);
		}

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
			if (word->text == str)
				return true;
		}
		
		return false;
	}

	bool Session::isWordOverlapping(const Word& word) const
	{
		constexpr int minDistance {15};

		for (const auto& currentWord : m_words)
		{
			bool isInSameRow {currentWord->y == word.y};

			bool isOverlappingInX
			{
				word.x < getWordEndPosition(*currentWord) &&
				getWordEndPosition(word) > currentWord->x
			};

			bool isWithinMinDistance
			{
				isWithinRange(word.x, getWordEndPosition(*currentWord), minDistance) ||
				isWithinRange(getWordEndPosition(word), currentWord->x, minDistance)
			};

			if ((isInSameRow && isOverlappingInX) || (isInSameRow && isWithinMinDistance))
				return true;
		}

		return false;
	}

	std::optional<Word> Session::getRandomWord() const
	{
		std::string str {};

		// Ensure that on screen words are different
		while (str.empty() || isWordPresent(str))
			str = Random::getElement(*m_wordBank);

		Word word {str};

		constexpr int minOffset {10};
		constexpr int maxOffset {100};

		static const std::vector<int> s_canvasRows {std::move(getCanvasRows())};

		// Prevents infinite overlap check
		int repositionCount {10};

		do
		{
			int xPos {getWordStartPosition(str) - Random::get(minOffset, maxOffset)};
			int yPos {Random::getElement(s_canvasRows)};

			// Reposition the word
			word.x = xPos;
			word.y = yPos;

			repositionCount--;

		} while (isWordOverlapping(word) && repositionCount > 0);

		return (repositionCount <= 0 ? std::optional<Word> {} : word);
	}

	void Session::addWords()
	{
		constexpr int spawnCount {5};

		for (int i {0}; i < spawnCount; ++i)
		{
			auto word {getRandomWord()};

			if(word)
				m_words.push_back(std::make_unique<Word>(std::move(*word)));
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
			if (word->x >= g_canvasWidth)
			{
				++m_misses;
				return true;
			}

			return false;
		});
	}
}