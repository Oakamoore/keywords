#ifndef KEYWORDS_AUDIO_H
#define KEYWORDS_AUDIO_H

#include "miniaudio.h"
#include <filesystem>

namespace Keywords
{
	namespace Audio
	{
		enum TrackID : std::size_t
		{
			main_menu,
			session_slow,
			session_medium,
			session_fast,
			leaderboard,
			max_track_id
		};

		class Track
		{
		public:
			Track() = default;
			Track(const std::filesystem::path& filePath);
			~Track();

			void play();
			void stop();
			bool isPlaying() const { return m_isPlaying; }

			Track(const Track&) = delete;
			Track& operator=(const Track&) = delete;

		private:
			ma_decoder m_decoder {};
			ma_device_config m_config {};
			ma_device m_device {};
			bool m_isPlaying {};

		};
	}
}

#endif