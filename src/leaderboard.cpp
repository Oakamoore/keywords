#include "leaderboard.h"
#include "session.h"
#include "util.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <cctype>

namespace
{
	/*
	* // Returns an individual line (non-empty) to be passed into 'separateEntries()'
	* std::string_view readEntriesFromFile(const std::filesystem::path& saveFilePath)
	*/
	
	/*
	* Splits the save entry into strings using the separator delimiter
	* std::vector<std::string> separateEntries(std::string_view entry)
	*/

	void formatTable(ftxui::Table& table)
	{
		constexpr int minColumnWidth {12};

		// Center all the text, and set a minimum column width
		table.SelectAll().DecorateCells(ftxui::center);
		table.SelectColumns(1, -1).Decorate(ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, minColumnWidth));

		// Add a border to the table, and space out the rows
		table.SelectAll().Border(ftxui::ROUNDED);
		table.SelectAll().Separator(ftxui::EMPTY);

		// Add a border to the first column
		table.SelectColumn(0).Border();

		// Add a separator between each row
		table.SelectRows(0, -1).SeparatorHorizontal();

		// Embolden the first row and column
		table.SelectRow(0).Decorate(ftxui::bold);
		table.SelectColumn(0).Decorate(ftxui::bold);
		
		// Set the colours of given cells
		table.SelectRow(0).DecorateCells(ftxui::color(ftxui::Color::GrayDark));
		table.SelectColumn(0).DecorateCells(ftxui::color(ftxui::Color::GrayDark));
	}

	ftxui::Element getTable([[maybe_unused]] const Keywords::SessionConfig& config)
	{
		// Form a 'std::vector<std::vector<std::string>>' using the above functions
		std::vector<std::vector<std::string>> entries(10, std::vector<std::string>(9));

		// Headings
		entries[0] = {"   ", "Score", "Words Typed", "Chars Typed", "WPM", "CPS", "Time", "Date-Time", "Username"};

		// Fill vector
		for (std::size_t i {1}; i < entries.size(); ++i)
		{
			for (std::size_t j {0}; j < entries[i].size(); ++j)
			{
				if (j != 0)
					entries[i][j] = "empty";
				else
					entries[i][j] = std::to_string(i);
			}
		}

		// Test data
		entries[1] = {"1", "1504", "41", "184", "37", "2.78391", "66.09", "2024-10-23 20:22:50", "Oakamoore"};

		auto table {ftxui::Table{entries}};

		formatTable(table);

		return table.Render();
	}

	/*
	* Appends a string to the last entry in the save file
	* void appendNameToEntry(const std::filesystem::path& saveFilePath, std::string_view name);
	*/
}

namespace Keywords
{
	void Leaderboard::handleInput(InputComponent& inputComponent, [[maybe_unused]] const std::filesystem::path& saveFilePath,
								  std::function<void()> quit, std::function<void()> save)
	{
		if (inputComponent.hasPressedEscape)
		{
			/*appendNameToEntry(saveFilePath) - append "Untitled"*/
			quit();
		}

		/*
		* 
		* if(inputComponent.hasPressedEnter)
		* {
		*	if(inputComponent.contents.empty())
		*	{
		*		appendNameToEntry(saveFilePath, "Untitled");
		*	}
		*	else
		*	{
		*		appendNameToEntry(saveFilePath, inputComponent.contents);
		*	}
		*	
		*	// Maybe wait a while before calling this
		*	save();
		* }
		*/
	}

	ftxui::Component getLeaderboard(const SessionConfig& config, [[maybe_unused]] InputComponent& inputComponent)
	{
		auto component {ftxui::Renderer(inputComponent.component, [&]
		{
			// Placed in the lambda body to prevent it dangling 
			auto difficulty {Util::getStringFromDifficulty(config.difficulty)};
			
			return ftxui::vbox
			({
				ftxui::filler(),
				ftxui::hbox
				({
					ftxui::text("LEADERBOARD"),
					ftxui::text(" ("),
					ftxui::text(Util::convertToCase(difficulty, ::toupper)) | ftxui::color(ftxui::Color::Cyan),
					ftxui::text(")")
				}) | ftxui::center,

				ftxui::filler(), getTable(config), ftxui::filler(),
				inputComponent.draw(), ftxui::filler(),
			 });
		})};

		return component;
	}
}