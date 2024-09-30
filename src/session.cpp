#include "session.h"
#include "random.h"
#include "word_bank.h"
#include "constants.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <sstream>
#include <type_traits>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <array>

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

	std::string_view getStringFromDifficulty(Keywords::SessionConfig::Difficulty difficulty)
	{
		using enum Keywords::SessionConfig::Difficulty;
		using namespace Keywords::Constants;

		switch (difficulty)
		{
			case medium:	return	difficultyOptions[static_cast<std::size_t>(medium)];
			case hard:		return	difficultyOptions[static_cast<std::size_t>(hard)];
			default:		return	difficultyOptions[static_cast<std::size_t>(easy)];
		}
	}

	std::string convertToUppercase(std::string_view str)
	{
		if (!str.empty())
		{
			std::string uppercase(str.length(), ' ');

			std::transform(str.begin(), str.end(), uppercase.begin(), [] (unsigned char c)
			{
				return static_cast<char>(std::toupper(c));
			});

			return uppercase;
		}

		return std::string {};
	}
}

namespace Keywords
{
	Session::Session(const SessionConfig& config, std::function<void()> back)
		: m_config {config}
		, m_back {back}
		, m_difficulty {getStringFromDifficulty(m_config.difficulty)}
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

	ftxui::Element Session::draw() const
	{
		// Create a blank canvas
		ftxui::Canvas c {g_canvasWidth, g_canvasHeight};

		// Draw the words to the canvas
		for (const auto& word : m_words)
			c.DrawText(word->x, word->y, word->text, word->color);

		constexpr int inputBoxWidth {g_canvasWidth / 6};
		constexpr int statBoxWidth {(g_canvasWidth / g_canvasCellWidth) - inputBoxWidth};

		using namespace ftxui;

		auto frame
		{
			vbox
			({
				text("KEYWORDS") | center,
				separatorEmpty(),
				canvas(std::move(c)) | border,
				hbox
				({
					m_input.draw() | border | size(WIDTH, EQUAL, inputBoxWidth),
					separatorEmpty(),
					window
					(
						hbox
						({
							text("Session Stats ("),
							text(convertToUppercase(m_difficulty)) | color(Color::Cyan),
							text(")")
						}) | center,

						hbox
						({
							filler(),
							createStatElement("Time: ", m_uptime.elapsed(), 's'), filler(),
							createStatElement("WPM: ", m_wordsPerMinute), filler(),
							createStatElement("CPS: ", m_charsPerSecond), filler(),
							createStatElement("Misses: ", m_misses), filler(),
						})

					) | size(WIDTH, EQUAL, statBoxWidth),
				}), separatorEmpty()
			}) | size(WIDTH, EQUAL, g_canvasWidth / g_canvasCellWidth)
		};

		return frame;
	}

	void Session::update()
	{
		/*
		constexpr int maxMisses {10};

		if(m_misses >= maxMisses)
			// m_lose();
		*/

		// Update word position and color
		for (auto& word : m_words)
		{
			word->move();
			word->updateColor(g_canvasWidth);
		}
		
		constexpr static std::array<double, SessionConfig::difficultyCount> spawnDelays {3.5, 4.5, 5.5};

		// The session has just begun, or the delay between spawns has passed 
		if (m_timeStamp == 0.0 || (m_uptime.elapsed() - m_timeStamp) >= 
			spawnDelays[static_cast<std::size_t>(m_config.difficulty)])
		{
			addWords();
			m_timeStamp = m_uptime.elapsed();
		}

		handleInput();
		updateStats();
		eraseWords();
	}

	bool Session::isWordPresent(std::string_view str) const
	{
		if (str.empty())
			return false;

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

			if (isInSameRow && (isOverlappingInX || isWithinMinDistance))
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
		constexpr int spawnCount {3};

		for (int i {0}; i < spawnCount; ++i)
		{
			auto word {getRandomWord()};

			if (word)
				m_words.push_back(std::make_unique<Word>(std::move(*word)));
		}
	}

	void Session::eraseWords()
	{
		std::erase_if(m_words, [&] (const auto& word)
		{
			// Erase off screen words
			if (word->x >= g_canvasWidth)
			{
				++m_misses;
				return true;
			}

			return false;
		});
	}

	void Session::handleInput()
	{
		// Quit the current session
		if (m_input.hasPressedEscape)
			m_back();

		if (m_input.hasPressedEnter)
		{
			// Check against input, 
			// erasing any matching words
			if (isWordPresent(m_input.content))
			{
				auto word {std::ranges::find_if(m_words,[&] (const auto& word)
				{
					if (word->text == m_input.content)
						return true;

					return false;
				})};

				if (word != m_words.end())
					m_words.erase(word);

				++m_wordsTyped;
				m_charsTyped += static_cast<int>(m_input.content.length());
			}

			// Clear the input component
			m_input.reset();
		}
	}

	void Session::updateStats()
	{
		m_wordsPerMinute = static_cast<int>(m_wordsTyped / (m_uptime.elapsed() / 60));
		m_charsPerSecond = m_charsTyped / m_uptime.elapsed();
	}
}