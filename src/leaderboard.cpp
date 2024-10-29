#include "leaderboard.h"

//namespace
//{
//	constexpr int g_leaderboardRows {15};
//	constexpr int g_leaderboardCols {9};
//
//	const std::vector<std::string> g_rowHeadings
//	{
//		{{}, "Score", "Words Typed", "Chars Typed", "WPM", "CPS", "Time", "Date-Time", "Username"}
//	};
//
//	/*
//	* // Returns an individual line (non-empty) to be passed into 'separateEntries()'
//	* std::string_view readEntriesFromFile(const std::filesystem::path& saveFilePath)
//	*/
//	
//	std::vector<std::string> readLinesFromFile(std::ifstream& file)
//	{
//		std::string line {};
//		std::vector<std::string> lines {};
//
//		while (std::getline(file, line))
//		{
//			// Skip empty lines
//			if (line.empty())
//				continue;
//
//			lines.push_back(line);
//		}
//		
//		return lines;
//	}
//
//	std::vector<std::string> separateStatsFromLine(const std::string& line)
//	{
//		std::istringstream stream {line};
//
//		std::string stat {};
//		std::vector<std::string> entry {};
//
//		// Separates the stats by a a given delimiter
//		while (std::getline(stream, stat, Keywords::Constants::statSeparator))
//			entry.push_back(stat);
//
//		return entry;
//	}
//
//	std::vector<std::vector<std::string>> readEntriesFromFile(const std::filesystem::path& saveFilePath)
//	{
//		if (!std::filesystem::exists(saveFilePath))
//			throw std::runtime_error("Unable to locate save file");
//
//		std::ifstream file {saveFilePath};
//
//		if (!file.is_open())
//			throw std::runtime_error("Failed to open save file");
//
//		std::vector<std::vector<std::string>> entries {};
//
//		for (const auto& line : readLinesFromFile(file))
//			entries.emplace_back(separateStatsFromLine(line));
//
//		return entries;
//	}
//
//	std::vector<std::vector<std::string>> getSortedEntries(const std::vector<std::vector<std::string>>& entries)
//	{
//		std::vector<std::vector<std::string>> sortedEntries(g_leaderboardRows + 1);
//
//		// Sort entries by score in descending order
//		std::ranges::partial_sort_copy(entries, sortedEntries, [] (const auto& a, const auto& b)
//		{
//			return std::stoi(a[0]) > std::stoi(b[0]);
//		});
//
//		return sortedEntries;
//	}
//
//	void formatTable(ftxui::Table& table)
//	{
//		constexpr int minColumnWidth {12};
//
//		// Center all the text, and set a minimum column width
//		table.SelectAll().DecorateCells(ftxui::center);
//		table.SelectColumns(1, -1).Decorate(ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, minColumnWidth));
//
//		// Add a border to the table, and space out the rows
//		table.SelectAll().Border(ftxui::ROUNDED);
//		table.SelectAll().Separator(ftxui::EMPTY);
//
//		// Add a border to the first column, and right align contents
//		table.SelectColumn(0).Border();
//		table.SelectColumn(0).DecorateCells(ftxui::align_right);
//
//		// Add a separator between each row
//		table.SelectRows(0, -1).SeparatorHorizontal();
//
//		// Embolden the first row and column
//		table.SelectRow(0).Decorate(ftxui::bold);
//		table.SelectColumn(0).Decorate(ftxui::bold);
//		
//		// Set the colours of given cells
//		table.SelectRow(0).DecorateCells(ftxui::color(ftxui::Color::GrayDark));
//		table.SelectColumn(0).DecorateCells(ftxui::color(ftxui::Color::GrayDark));
//	}
//
//	ftxui::Element getTable([[maybe_unused]] const Keywords::SessionConfig& config)
//	{
//		auto sortedEntries {getSortedEntries(readEntriesFromFile(Keywords::Util::getFilePathFromDifficulty(config.difficulty)))};
//
//		sortedEntries[0] = g_rowHeadings;
//
//		/*for (std::size_t i {1}; i < sortedEntries.size(); ++i)
//		{
//			for (std::size_t j {0}; j < sortedEntries[i].size(); ++j)
//			{
//				if (j != 0)
//					sortedEntries[i][j] = "empty";
//				else
//					sortedEntries[i][j] = std::to_string(i);
//			}
//		}*/
//
//		auto table {ftxui::Table{sortedEntries}};
//
//		formatTable(table);
//
//		return table.Render();
//	}
//
//	/*
//	* Appends a string to the last entry in the save file
//	* void appendUsernameToEntry(const std::filesystem::path& saveFilePath, std::string_view name);
//	*/
//}

namespace Keywords
{
	Leaderboard::Leaderboard(const SessionConfig& config, const std::filesystem::path& saveFilePath)
		: m_config {config}
		, m_saveFilePath {saveFilePath}
	{

	}

	ftxui::Element Leaderboard::draw() const
	{

	}

	void Leaderboard::update()
	{

	}

	bool Leaderboard::isHighScorePresent() const
	{

	}

	std::vector<std::string> Leaderboard::readLinesFromFile(std::ifstream& file) const
	{

	}

	std::vector<std::string> Leaderboard::separateStatsFromLine(const std::string& line) const
	{

	}

	void Leaderboard::formatTable(ftxui::Table& table)
	{

	}

	void Leaderboard::getEntriesFromFile()
	{

	}

	void Leaderboard::getSortedEntries()
	{

	}

	void Leaderboard::handleInput()
	{

	}

	//void Leaderboard::handleInput(InputComponent& inputComponent, [[maybe_unused]] const std::filesystem::path& saveFilePath,
	//							  std::function<void()> quit, std::function<void()> save)
	//{
	//	if (inputComponent.hasPressedEscape)
	//	{
	//		/*appendNameToEntry(saveFilePath) - append "Untitled"*/
	//		quit();
	//	}

	//	/*
	//	* // Only if the most recent entry is a highscore
	//	* if(inputComponent.hasPressedEnter)
	//	* {
	//	*	if(inputComponent.contents.empty())
	//	*	{
	//	*		appendNameToEntry(saveFilePath, "Untitled");
	//	*	}
	//	*	else
	//	*	{
	//	*		appendNameToEntry(saveFilePath, inputComponent.contents);
	//	*	}
	//	*	
	//	*	// Maybe wait a while before calling this
	//	*	save();
	//	* }
	//	*/
	//}

	//ftxui::Component getLeaderboardComponent(const SessionConfig& config, [[maybe_unused]] InputComponent& inputComponent)
	//{
	//	constexpr int inputBoxWidth {20};
	//	constexpr int inputBoxHeight {3};
	//	
	//	using namespace ftxui;

	//	auto component {Renderer(inputComponent.component, [&]
	//	{
	//		// Placed in the lambda body to prevent it dangling 
	//		auto difficulty {Util::getStringFromDifficulty(config.difficulty)};
	//		
	//		return vbox
	//		({
	//			filler(),
	//			hbox
	//			({
	//				text("LEADERBOARD"),
	//				text(" ("),
	//				text(Util::convertToCase(difficulty, ::toupper)) | color(Color::Cyan),
	//				text(")"), 
	//			}) | center, filler(),
	//			getTable(config) | center, filler(),
	//			inputComponent.draw() | border 
	//			| size(WIDTH, EQUAL, inputBoxWidth)
	//			| size(HEIGHT, EQUAL, inputBoxHeight)
	//			| notflex | center, filler(),
	//		 });
	//	})};

	//	return component;
	//}
}