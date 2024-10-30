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

		/*
		* 
		*	// The most recent entry is not a high score
		*	m_input.component |= ftxui::Maybe(isHighScorePresent());
		* 
		*/
	}

	ftxui::Element Leaderboard::draw()
	{
		ftxui::Table table {std::move(getTable(m_sortedEntries))};

		formatTable(table);

		constexpr int inputBoxWidth {20};
		constexpr int inputBoxHeight {3};

		auto difficulty {Util::getStringFromDifficulty(m_config.difficulty)};
		
		using namespace ftxui;

		auto frame
		{
			vbox
			({
				filler(),
				hbox
				({
					text("LEADERBOARD"),
					text(" ("),
					text(Util::convertToCase(difficulty, ::toupper)) | color(Color::Cyan),
					text(")"),
				}) | center, filler(),
				table.Render() | center, filler(),
				m_input.draw() | border
				| size(WIDTH, EQUAL, inputBoxWidth)
				| size(HEIGHT, EQUAL, inputBoxHeight)
				| notflex | center, filler(),
			 })
		};

		return frame;
	}

	void Leaderboard::update()
	{
		handleInput();
	}

	bool Leaderboard::isHighScorePresent() const
	{
		// Return 'true' if 'm_sortedEntries' is full
		// and the score for the most recent entry 
		// isn't greater than any in 'm_sortedEntries'

		return false;
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

		// Add a border to the first column, and right align contents
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
		table.SelectColumn(-1).DecorateCells(ftxui::color(ftxui::Color::Yellow));
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

	void Leaderboard::handleInput()
	{
		// std::string_view placeholder {"Untitled"}

		if (m_input.hasPressedEscape)
		{
			m_quit();

			//appendNameToEntry(placeholder);
		}

		//if (m_input.hasPressedEscape)
		//{
		//	if(isHighScorePresent() && !m_input.content.empty())
		//		appendNameToEntry(m_input.content);
		//	else
		//		appendNameToEntry(placeholder); 
		//}
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
