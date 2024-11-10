#include "session.h"
#include "random.h"
#include "word_bank.h"
#include "constants.h"
#include "util.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <sstream>
#include <type_traits>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <array>
#include <fstream>
#include <exception>
#include <ostream>
#include <chrono>
#include <format>

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

	std::ostream& operator<<(std::ostream& out, const Keywords::SessionStats& stats)
	{
		auto c {Keywords::Constants::statSeparator};

		out << stats.score << c << stats.wordsTyped << c 
			<< stats.charsTyped << c << stats.wordsPerMinute << c 
			<< toStringWithPrecision(stats.charsPerSecond, 2) << c
			<< toStringWithPrecision(stats.totalTime, 2) << c;

		return out;
	}

	std::string getFormattedDateTime()
	{
		auto const dateTime {std::chrono::current_zone()->to_local(std::chrono::system_clock::now())};

		return std::format("{:%Y-%m-%d %X}", dateTime);
	}

	double getSpawnDelay(const Keywords::SessionConfig::Difficulty difficulty, int wordsTyped)
	{
		constexpr std::array<double, Keywords::SessionConfig::max_difficulty> spawnDelays {3.5, 4.5, 6.0};
		constexpr std::array delayModifiers {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
		constexpr std::array<int, Keywords::SessionConfig::max_difficulty> wordSteps {20, 15, 10};

		// Every 'wordStep' words typed
		// the spawn delay is decreased
		int index {wordsTyped / wordSteps[difficulty]};

		if (index == 0)
			return spawnDelays[difficulty];								// Base delay
		else if (index - 1 >= delayModifiers.size())
			return spawnDelays[difficulty] - delayModifiers.back();		// Smallest delay
		else
			return spawnDelays[difficulty] - delayModifiers[index - 1]; // Decreased delay
	}
}

namespace Keywords
{
	Session::Session(const SessionConfig& config, const std::filesystem::path& saveFilePath, std::function<void()> back, std::function<void()> lose)
		: m_config {config}
		, m_saveFilePath {saveFilePath}
		, m_back {back}
		, m_lose {lose}
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

		m_stats.difficulty = Util::getStringFromDifficulty(config.difficulty);
	}

	ftxui::Element Session::draw() const
	{
		// Create a blank canvas
		ftxui::Canvas c {g_canvasWidth, g_canvasHeight};

		// Draw the words to the canvas
		for (const auto& word : m_words)
			c.DrawText(word->x, word->y, word->text, word->color);

		constexpr int inputBoxWidth {g_canvasWidth / 6};
		constexpr int inputBoxHeight {5};
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
					m_input.draw() | border 
					| size(WIDTH, EQUAL, inputBoxWidth) 
					| size(HEIGHT, EQUAL, inputBoxHeight),
					separatorEmpty(),
					window
					(
						hbox
						({
							text("Session Stats ("),
							text(Util::convertToCase(m_stats.difficulty, ::toupper)) | color(Color::Cyan),
							text(")")
						}) | center,

						hbox
						({
							filler(),
							createStatElement("Time: ", m_uptime.elapsed(), 's'), filler(),
							createStatElement("Score: ", m_stats.score), filler(),
							createStatElement("WPM: ", m_stats.wordsPerMinute), filler(),
							createStatElement("CPS: ", m_stats.charsPerSecond), filler(),
							createStatElement("Misses: ", m_misses), filler(),
						})

					) | size(WIDTH, EQUAL, statBoxWidth),
				}), separatorEmpty()
			}) | size(WIDTH, EQUAL, g_canvasWidth / g_canvasCellWidth)
		};

		return frame | center;
	}

	void Session::update()
	{
		updateStats();

		if (m_misses >= Constants::maxMisses)
		{
			m_lose();
			writeToFile();
			return;
		}

		// Update word position and color
		for (auto& word : m_words)
		{
			word->move();
			word->updateColor(g_canvasWidth);
		}

		// The session has just begun, or the delay between spawns has passed 
		if (m_timeStamp == 0.0 || (m_uptime.elapsed() - m_timeStamp) >= getSpawnDelay(m_config.difficulty, m_stats.wordsTyped))
		{
			addWords();
			m_timeStamp = m_uptime.elapsed();
		}

		handleInput();
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
			if (isWordPresent(m_input.content))
			{
				constexpr int scoreModifier {5};

				auto word {std::ranges::find_if(m_words,[&] (const auto& word)
				{
					if (word->text == m_input.content)
						return true;

					return false;
				})};

				if (word != m_words.end())
				{
					// Less points are gained by typing a word further along the canvas
					m_stats.score += (static_cast<int>(m_input.content.length()) + g_canvasWidth - word->get()->x) / scoreModifier;
					m_words.erase(word);
				}

				++m_stats.wordsTyped;
				m_stats.charsTyped += static_cast<int>(m_input.content.length());
			}

			// Clear the input component
			m_input.reset();
		}
	}

	void Session::updateStats()
	{
		m_stats.wordsPerMinute = static_cast<int>(m_stats.wordsTyped / (m_uptime.elapsed() / 60));
		m_stats.charsPerSecond = m_stats.charsTyped / m_uptime.elapsed();
	}

	void Session::writeToFile()
	{
		if (!std::filesystem::exists(m_saveFilePath))
			throw std::runtime_error("Unable to locate save file");

		std::ofstream file {m_saveFilePath, std::ios::app};

		if (!file.is_open())
			throw std::runtime_error("Failed to open save file");

		m_stats.totalTime = m_uptime.elapsed();
		
		// Average WPM and CPS
		m_stats.wordsPerMinute = static_cast<int>(m_stats.wordsTyped / (m_stats.totalTime / 60));
		m_stats.charsPerSecond = m_stats.charsTyped / m_stats.totalTime;

		// The hypen string represents a blank username
		file << m_stats << getFormattedDateTime()  << Constants::statSeparator << std::string {"-----"} << '\n';

		file.close();
	}

	ftxui::Component getSessionComponent(Session& session)
	{
		auto component {ftxui::Renderer(session.getInputComponent().component, [&]
		{
			{
				return session.draw();
			}
		})};

		return component;
	}
}