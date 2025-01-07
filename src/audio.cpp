#include "audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <exception>

namespace
{
	void dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount)
	{
		ma_decoder* decoder {(ma_decoder*) device->pUserData};

		if (decoder == nullptr)
			return;

		ma_data_source_read_pcm_frames(decoder, output, frameCount, nullptr);

		(void) input;
	}
}

namespace Keywords
{
	namespace Audio
	{
		Track::Track(const std::filesystem::path& filePath)
		{
			auto result {ma_decoder_init_file(filePath.string().c_str(), nullptr, &m_decoder)};

			if (result != MA_SUCCESS)
				throw std::runtime_error("Unable to open \"" + filePath.filename().string() + "\" audio file");

			// Loop the audio source
			ma_data_source_set_looping(&m_decoder, MA_TRUE);

			m_config = ma_device_config_init(ma_device_type_playback);
			m_config.playback.format = m_decoder.outputFormat;
			m_config.playback.channels = m_decoder.outputChannels;
			m_config.sampleRate = m_decoder.outputSampleRate;
			m_config.dataCallback = dataCallback;
			m_config.pUserData = &m_decoder;
		}

		Track::~Track()
		{
			ma_device_uninit(&m_device);
			ma_decoder_uninit(&m_decoder);
		}

		void Track::play()
		{
			if (ma_device_init(nullptr, &m_config, &m_device) != MA_SUCCESS)
				throw std::runtime_error("Failed to initialise playback device");

			if (ma_device_start(&m_device) != MA_SUCCESS)
			{
				ma_device_uninit(&m_device);
				ma_decoder_uninit(&m_decoder);
				throw std::runtime_error("Failed to start playback device");
			}

			m_isPlaying = true;
		}

		void Track::stop()
		{
			if (ma_device_is_started(&m_device))
				ma_device_stop(&m_device);

			m_isPlaying = false;
		}
	}
}