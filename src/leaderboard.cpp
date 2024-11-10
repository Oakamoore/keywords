#include "leaderboard.h"
#include "constants.h"
#include "util.h"
#include <ftxui/dom/elements.hpp>
#include <fstream>
#include <algorithm>

namespace
{
	constexpr int g_leaderboardRows {16};
	constexpr int g_leaderboardCols {9};
	constexpr int g_inputLength {10};

	std::vector<std::string> readLinesFromFile(std::ifstream& file)
	{
		std::string line {};
		std::vector<std::string> lines {};

		while (std::getline(file, line))
		{
			// Skip empty lines
			if (line.empty())
				continue;

			lines.push_back(line);
		}

		return lines;
	}

	std::vector<std::string> separateStatsFromLine(const std::string& line)
	{
		std::istringstream stream {line};

		std::string stat {};
		std::vector<std::string> entry {};

		// Separates the stats by a a given delimiter
		while (std::getline(stream, stat, Keywords::Constants::statSeparator))
			entry.push_back(stat);

		return entry;
	}

	ftxui::Table getTable(const std::vector<std::vector<std::string>>& entries)
	{
		std::vector<std::vector<std::string>> table(g_leaderboardRows, std::vector<std::string>(g_leaderboardCols));

		// Column headings
		table[0] = {{}, "Score", "Words Typed", "Chars Typed", "WPM", "CPS", "Time", "Date-Time", "Username"};

		for (std::size_t row {1}; row < g_leaderboardRows; ++row)
		{
			// Row numbers
			table[row][0] = std::to_string(row);

			for (std::size_t col {1}; col < g_leaderboardCols; ++col)
			{
				// Ignore unpopulated entries
				if (!entries[row - 1].empty())
				{
					// Copy over the session entry data
					table[row][col] = entries[row - 1][col - 1];
				}
			}
		}

		return table;
	}

	ftxui::Element getDescription(bool hasEnterPrompt)
	{
		using namespace ftxui;

		auto description
		{
			vbox
			({
				hbox({text("Press "), text("ESCAPE") | color(Color::Yellow), text(" to exit from the leaderboard.")}) | center,
				(hasEnterPrompt ? hbox({text("Press "), text("ENTER") | color(Color::Yellow), text(" to confirm a typed username (<= " 
								+ std::to_string(g_inputLength) + " characters).")}) : emptyElement()) | center
			}),
		};

		return description;
	}

	ftxui::Element getSingleEntry(const std::vector<std::string>& entry)
	{
		ftxui::Elements elements {};

		for (std::size_t i {}; i < entry.size() - 1; ++i)
		{
			auto separator {i != entry.size() - 2 ? ',' : '\0'};

			elements.push_back(ftxui::text("  " + entry[i] + separator));
		}

		auto stats
		{
			ftxui::vbox
			({
				ftxui::text("Most Recent Session:") | ftxui::center | ftxui::color(ftxui::Color::GrayDark),
				ftxui::hbox({elements}) | ftxui::center | ftxui::color(ftxui::Color::GrayDark)
			 })
		};

		return stats;
	}

	void appendStringToFile(const std::filesystem::path& filePath, std::string_view str)
	{
		if (!std::filesystem::exists(filePath))
			throw std::runtime_error("Unable to locate save file");

		std::ifstream input {filePath};

		if (!input.is_open())
			throw std::runtime_error("Failed to open save file");

		std::string line {};
		std::string contents {};

		// Read the entire file 
		while (std::getline(input, line))
			contents += line + '\n';

		auto editIndex {contents.find_last_of(Keywords::Constants::statSeparator)};

		// Remove the blank username from the most recent entry
		if (editIndex != std::string::npos)
			contents = contents.substr(0, editIndex);

		input.close();

		// Overwrite the save file 
		std::ofstream output {filePath};

		if (!output.is_open())
			throw std::runtime_error("Failed to open save file");

		// Append the username to the save file data
		contents += Keywords::Constants::statSeparator + std::string {str} + '\n';

		output << contents;

		output.close();
	}
}

namespace Keywords
{
	Leaderboard::Leaderboard(const SessionConfig& config, const std::filesystem::path& saveFilePath, std::function<void()> quit)
		: m_config {config}
		, m_saveFilePath {saveFilePath}
		, m_sortedEntries {g_leaderboardRows - 1}	
		, m_quit {quit}
		, m_input {"Enter Username", false} 
	{
		getEntriesFromFile();
		sortEntries();

		m_isInputNeeded = isHighScorePresent();
	}

	ftxui::Element Leaderboard::draw()
	{
		ftxui::Table table {std::move(getTable(m_sortedEntries))};

		formatTable(table);

		constexpr int inputBoxWidth {20};
		constexpr int inputBoxHeight {3};

		auto difficulty {Util::getStringFromDifficulty(m_config.difficulty)};
		
		using namespace ftxui;

		auto inputBox
		{
			m_input.draw() | border | size(WIDTH, EQUAL, inputBoxWidth)
			| size(HEIGHT, EQUAL, inputBoxHeight) | notflex | center
		};

		auto frame
		{
			vbox
			({
				separatorEmpty(), filler(),
				hbox
				({
					text("LEADERBOARD"),
					text(" ("),
					text(Util::convertToCase(difficulty, ::toupper)) | color(Color::Cyan),
					text(")"),
				}) | center, filler(),
				(m_isInputNeeded ? emptyElement() : separatorEmpty()),
				table.Render() | center, filler(),
				(!isHighScorePresent() ? getSingleEntry(m_unsortedEntries.back()) : emptyElement()), filler(),
				getDescription(m_isInputNeeded), filler(),
				(m_isInputNeeded ? inputBox : emptyElement()), filler(),
			 })
		};

		return frame;
	}

	void Leaderboard::handleInput()
	{
		if (m_input.hasPressedEscape)
			m_quit();

		if (m_input.hasPressedEnter)
		{
			if (m_isInputNeeded && (!m_input.content.empty() && m_input.content.size() <= g_inputLength))
			{
				appendStringToFile(m_saveFilePath, m_input.content);

				// Hide the input component
				m_isInputNeeded = false;

				m_unsortedEntries.clear();

				// Update leaderboard entries
				getEntriesFromFile();
				sortEntries();
			}
		}
	}
	
	bool Leaderboard::isHighScorePresent() const
	{
		// The number of entries equals or exceeds highscore count
		auto isFull {std::ranges::all_of(m_sortedEntries, [&] (const auto& a) { return !a.empty(); })};
		
		// Whether the most recent entry is a highscore
		auto isPresent {std::ranges::find(m_sortedEntries, m_unsortedEntries.back()) != m_sortedEntries.end()};

		return ((!isFull || isFull && isPresent) ? true : false);
	}

	void Leaderboard::formatTable(ftxui::Table& table)
	{
		constexpr int minColumnWidth {12};

		// Center all the text, and set a minimum column width
		table.SelectAll().DecorateCells(ftxui::center);
		table.SelectColumns(1, -1).Decorate(ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, minColumnWidth));

		// Add a border to the table, and space out the rows
		table.SelectAll().Border(ftxui::ROUNDED);
		table.SelectAll().Separator(ftxui::EMPTY);

		// Add a border to the first column, and right align its contents
		table.SelectColumn(0).Border();
		table.SelectColumn(0).DecorateCells(ftxui::align_right);

		// Add a separator between each row
		table.SelectRows(0, -1).SeparatorHorizontal();

		// Embolden the first row and column
		table.SelectRow(0).Decorate(ftxui::bold);
		table.SelectColumn(0).Decorate(ftxui::bold);

		// Set the colours of given cells
		table.SelectRow(0).DecorateCells(ftxui::color(ftxui::Color::GrayDark));
		table.SelectColumn(0).DecorateCells(ftxui::color(ftxui::Color::GrayDark));

		if (isHighScorePresent())
		{
			auto iter {std::ranges::find(m_sortedEntries, m_unsortedEntries.back())};

			// Position of the newest highscore
			// Offset to account for column headings
			auto rowIndex {static_cast<int>(std::distance(m_sortedEntries.begin(), iter)) + 1};

			// Highlight the high score
			table.SelectRow(rowIndex).DecorateCells(ftxui::color(ftxui::Color::Yellow));
		}
	}

	void Leaderboard::getEntriesFromFile()
	{
		if (!std::filesystem::exists(m_saveFilePath))
			throw std::runtime_error("Unable to locate save file");

		std::ifstream file {m_saveFilePath};

		if (!file.is_open())
			throw std::runtime_error("Failed to open save file");

		for (const auto& line : readLinesFromFile(file))
			m_unsortedEntries.emplace_back(separateStatsFromLine(line));

		file.close();
	}

	void Leaderboard::sortEntries()
	{
		// Sort entries by score in descending order
		std::ranges::partial_sort_copy(m_unsortedEntries, m_sortedEntries, [] (const auto& a, const auto& b)
		{
			return std::stoi(a[0]) > std::stoi(b[0]);
		});
	}

	ftxui::Component getLeaderboardComponent(Leaderboard& leaderboard)
	{
		auto component {ftxui::Renderer(leaderboard.getInputComponent().component, [&]
		{
			{
				return leaderboard.draw();
			}
		})};

		return component;
	}
}
