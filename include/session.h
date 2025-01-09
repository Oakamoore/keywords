#ifndef KEYWORDS_SESSION_H
#define KEYWORDS_SESSION_H

#include "game_config.h"
#include "input_component.h"
#include "word.h"
#include "timer.h"
#include "word_bank.h"
#include "audio.h"
#include <ftxui/dom/node.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <functional>
#include <filesystem>
#include <array>

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
		enum SessionTrack : std::size_t
		{
			slow_track,
			medium_track,
			fast_track,
			max_session_tracks
		};

		Session(const GameConfig& config, const WordBank& wordBank, 
				const std::filesystem::path& saveFilePath, 
				std::array<Audio::Track, max_session_tracks>& tracks, 
				std::function<void()> back, std::function<void()> lose);

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
		void playTracks();
		void stopTracks();

	private:
		const GameConfig m_config {};
		const std::filesystem::path m_saveFilePath {};
		const std::vector<std::string>* m_wordBank {};
		std::vector<std::unique_ptr<Word>> m_words {};
		Audio::Track* m_slowTrack {};
		Audio::Track* m_mediumTrack {};
		Audio::Track* m_fastTrack {};
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
