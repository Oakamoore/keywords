#ifndef KEYWORDS_LEADERBOARD_H
#define KEYWORDS_LEADERBOARD_H

#include "session.h"
#include "input_component.h"
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/table.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace Keywords
{
	class Leaderboard
	{
	public:
		Leaderboard(const SessionConfig& config, const std::filesystem::path& saveFilePath);

		ftxui::Element draw() const;
		void update();

	private:
		bool isHighScorePresent() const;

		std::vector<std::string> readLinesFromFile(std::ifstream& file) const;
		std::vector<std::string> separateStatsFromLine(const std::string& line) const;

		void formatTable(ftxui::Table& table);
		void getEntriesFromFile();
		void getSortedEntries();
		void handleInput();

	private:
		const SessionConfig m_config {};
		const std::filesystem::path m_saveFilePath {};
		std::vector<std::vector<std::string>> m_unsortedEntries {};
		std::vector<std::vector<std::string>> m_sortedEntries {};
		InputComponent m_input {};

	};

	/*namespace Leaderboard
	{
		void handleInput(InputComponent& inputComponent, const std::filesystem::path& saveFilePath,
						 std::function<void()> quit, std::function<void()> save);
	}*/
}

#endif