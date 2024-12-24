#ifndef KEYWORDS_SESSION_H
#define KEYWORDS_SESSION_H

#include "input_component.h"
#include "word.h"
#include "timer.h"
#include <ftxui/dom/node.hpp>
#include <ftxui/component/component.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <functional>
#include <filesystem>

namespace Keywords
{
	struct SessionStats
	{
		int score {};
		int wordsTyped {};
		int charsTyped {};
		int wordsPerMinute {};
		double charsPerSecond {};
		double totalTime {};
		std::string_view difficulty {};
	};

	class Session
	{
	public:
		Session(const SessionConfig& config, const std::filesystem::path& saveFilePath, std::function<void()> back, std::function<void()> lose);

		ftxui::Element draw() const;
		void update();

		InputComponent& getInputComponent() { return m_input; }

		friend struct SessionTest;

	private:
		bool isWordPresent(std::string_view str) const;
		bool isWordOverlapping(const Word& word) const;
		
		std::optional<Word> getRandomWord() const;
		double getSpawnDelay() const;

		void addWords();
		void eraseWords();
		void handleInput();
		void updateStats();
		void writeToFile();

	private:
		const SessionConfig m_config {};
		const std::filesystem::path m_saveFilePath {};
		const std::vector<std::string>* m_wordBank {};
		std::vector<std::unique_ptr<Word>> m_words {};
		std::function<void()> m_back {};
		std::function<void()> m_lose {};
		InputComponent m_input {};
		SessionStats m_stats {};
		double m_timeStamp {};
		Timer m_uptime {};
		int m_misses {};
	};
}

#endif
