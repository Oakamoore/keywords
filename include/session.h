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
#include <array>

namespace Keywords
{
	struct SessionConfig
	{
		enum class Difficulty
		{
			easy,
			medium,
			hard,
			max_difficulty
		};

		constexpr static auto difficultyCount {static_cast<std::size_t>(Difficulty::max_difficulty)};

		Difficulty difficulty {};
		std::array<int, difficultyCount> spawnCounts {3, 2, 1};
	
	};

	class Session
	{
	public:
		Session(const SessionConfig& config, std::function<void()> back);

		ftxui::Element draw() const;
		void update();

		ftxui::Component getInputComponent() const { return m_input.component; }
		
	private:
		bool isWordPresent(std::string_view str) const;
		bool isWordOverlapping(const Word& word) const;
		
		std::optional<Word> getRandomWord() const;

		void addWords();
		void eraseWords();
		void handleInput();
		void updateStats();

	private:
		const SessionConfig m_config {};
		std::string_view m_difficulty {};
		const std::vector<std::string>* m_wordBank {};
		std::vector<std::unique_ptr<Word>> m_words {};
		std::function<void()> m_back {};
		/*std::function<void()> m_lose {};*/
		InputComponent m_input {};
		Timer m_uptime {};
		double m_timeStamp {};
		int m_misses {};
		int m_charsTyped {};
		int m_wordsTyped {};
		int m_wordsPerMinute {};
		double m_charsPerSecond {};

	};
}

#endif
