#pragma once
#include "all.h"

namespace amun
{
	class Playback
	{
	public:
		Playback() : m_Device({}), m_SilenceDuration(250), m_Receiver(nullptr), m_IsRunning(false)
		{}

		~Playback()
		{
			destroy();
		}

		bool initialize(uint32_t sampleRate, uint8_t channelCount)
		{
			if (ma_device_get_state(&m_Device) != ma_device_state_uninitialized)
				return false;

			ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
			deviceConfig.playback.format = SAMPLE_FORMAT;
			deviceConfig.playback.channels = channelCount;
			deviceConfig.sampleRate = sampleRate;
			deviceConfig.dataCallback = dataCallback;
			deviceConfig.pUserData = this;

			if (ma_device_init(nullptr, &deviceConfig, &m_Device) != MA_SUCCESS)
			{
				notifyError(VOICE_CHAT_ERROR_FAILED_TO_INIT_PLAYBACK_DEVICE);
				return false;
			}

			m_SpeakerBuffers.clear();
			return true;
		}

		void destroy()
		{
			stop();
			ma_device_uninit(&m_Device);
			m_SpeakerBuffers.clear();
		}

		bool isInitialized() const
		{
			return ma_device_get_state(&m_Device) != ma_device_state_uninitialized;
		}

		bool start()
		{
			if (m_IsRunning)
				return true;

			if (ma_device_start(&m_Device) != MA_SUCCESS)
			{
				notifyError(VOICE_CHAT_ERROR_FAILED_TO_START_PLAYBACK_DEVICE);
				ma_device_uninit(&m_Device);
				m_IsRunning = false;
				return false;
			}

			m_IsRunning = true;
			return true;
		}

		void stop()
		{
			m_IsRunning = false;
			ma_device_stop(&m_Device);
		}

		void setVolume(float volume)
		{
			ma_device_set_master_volume(&m_Device, std::fminf(1.0f, std::fmaxf(0.0f, volume / 100.0f)));
		}

		void setSpeakerVolume(const std::string& speakerID, float volume)
		{
			m_SpeakerBuffers[speakerID].volume = std::fminf(1.0f, std::fmaxf(0.0f, volume / 100.0f));
		}

		void setSilenceDuration(uint32_t milliseconds)
		{
			m_SilenceDuration = milliseconds;
		}

		void registerSignalReceiver(SignalReceiver* receiver, uint8_t signal = 0)
		{
			m_Receiver = receiver;
			//m_ReceiverSignal = signal;
		}

		void unregisterSignalReceiver()
		{
			m_Receiver = nullptr;
			//m_ReceiverSignal = 0;
		}

		void addSamples(const std::string& speakerID, const SAMPLE_TYPE* data, size_t sampleCount)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			const auto search = m_SpeakerBuffers.find(speakerID);
			if (search == m_SpeakerBuffers.end())
			{
				m_SpeakerBuffers[speakerID].volume = 1.0;
			}

			SpeakerBuffer& buffer = m_SpeakerBuffers[speakerID];
			if (buffer.neededMoreSamples && buffer.samples.empty())
			{
				buffer.samples.insert(buffer.samples.end(), samplesPerMillisecond() * m_SilenceDuration, 0);
				buffer.neededMoreSamples = false;
			}

			buffer.samples.insert(buffer.samples.end(), data, data + sampleCount);
		}

	private:
		static void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
		{
			Playback* playback = (Playback*)pDevice->pUserData;
			if (!playback->getSamples(pOutput, frameCount * pDevice->playback.channels))
			{
				memset(pOutput, 0, frameCount * pDevice->playback.channels * sizeof(SAMPLE_TYPE));
			}
		}

		bool getSamples(void* out, uint32_t sampleCount)
		{
			static std::vector<SAMPLE_TYPE> mixedSamples(sampleCount, 0);
			if (sampleCount > mixedSamples.size())
				mixedSamples.resize(sampleCount);
			std::fill(mixedSamples.begin(), mixedSamples.end(), 0);

			bool hasData = false;
			std::lock_guard<std::mutex> lock(m_Mutex);
			for (auto& [_, buffer] : m_SpeakerBuffers)
			{
				if (buffer.samples.size() < sampleCount)
				{
					buffer.neededMoreSamples = true;
					buffer.samples.clear();
				}
				else
				{
					hasData = true;
					for (size_t i = 0; i < sampleCount; ++i)
					{
						int32_t sample = mixedSamples[i] + static_cast<int32_t>(buffer.samples[i] * buffer.volume);
						mixedSamples[i] = ma_clip_s16(sample);
					}
					buffer.samples.erase(buffer.samples.begin(), buffer.samples.begin() + sampleCount);
				}
			}

			if (hasData)
			{
				memcpy(out, mixedSamples.data(), sampleCount * sizeof(SAMPLE_TYPE));
			}
			else
			{
				memset(out, 0, sampleCount * sizeof(SAMPLE_TYPE)); // silence
			}

			return hasData;
		}

		uint32_t samplesPerMillisecond() const
		{
			return m_Device.sampleRate * m_Device.capture.channels / 1000;
		}

		void notifyError(uint8_t errorID)
		{
			if (m_Receiver)
			{
				m_Receiver->onError(errorID);
			}
		}

	private:
		ma_device m_Device;
		struct SpeakerBuffer
		{
			std::vector<SAMPLE_TYPE> samples;
			float volume = 1.0f;
			// Buffer might be empty when we receive the next batch of samples,
			// but that doesn't mean that the playback had already asked for more.
			// If it wasn't asked for more samples, then there's not point in adding
			// silence to try and combat delays/jitter
			bool neededMoreSamples;
		};
		std::mutex m_Mutex;
		std::unordered_map<std::string, SpeakerBuffer> m_SpeakerBuffers;
		uint32_t m_SilenceDuration;
		SignalReceiver* m_Receiver;
		//uint8_t m_ReceiverSignal;
		bool m_IsRunning;
	};
}
