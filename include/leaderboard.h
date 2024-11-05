#ifndef KEYWORDS_LEADERBOARD_H
#define KEYWORDS_LEADERBOARD_H

#include "session.h"
#include "input_component.h"
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/table.hpp>
#include <filesystem>
#include <string>
#include <vector>
#include <functional>

namespace Keywords
{
	class Leaderboard
	{
	public:
		Leaderboard(const SessionConfig& config, const std::filesystem::path& saveFilePath, std::function<void()> quit);

		ftxui::Element draw();
		void handleInput();

		InputComponent& getInputComponent() { return m_input; }

	private:
		bool isHighScorePresent() const;

		void formatTable(ftxui::Table& table, bool isHighlighted);
		void getEntriesFromFile();
		void sortEntries();

	private:
		const SessionConfig m_config {};
		const std::filesystem::path m_saveFilePath {};
		std::vector<std::vector<std::string>> m_unsortedEntries {};
		std::vector<std::vector<std::string>> m_sortedEntries {};
		std::function<void()> m_quit {};
		InputComponent m_input {};

	};
}

#endif