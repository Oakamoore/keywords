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

		ftxui::Element draw();
		void update();

		InputComponent& getInputComponent() { return m_input; }

	private:
		bool isHighScorePresent() const;

		void formatTable(ftxui::Table& table);
		void getEntriesFromFile();
		void sortEntries();
		void handleInput();

	private:
		const SessionConfig m_config {};
		const std::filesystem::path m_saveFilePath {};
		std::vector<std::vector<std::string>> m_unsortedEntries {};
		std::vector<std::vector<std::string>> m_sortedEntries {};
		InputComponent m_input {};

	};
}

#endif