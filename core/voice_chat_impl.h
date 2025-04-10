// Private implementation
#pragma once
#include "all.h"
#include "voice_chat.h"


namespace amun
{
	enum EVoiceChatImplSignals
	{
		VOICE_CHAT_SIGNAL_SAMPLES_READY,
	};

	class VoiceChatImpl : public SignalReceiver
	{
	public:
		VoiceChatImpl(VoiceChat* parent)
			: m_Parent(parent), m_SampleRate(0), m_ChannelCount(0), m_PlaybackDelay(250), m_TargetSpl(0)
		{}

		~VoiceChatImpl()
		{
			m_Parent = nullptr;
		}

		bool init(uint32_t sampleRate, uint8_t channelCount)
		{
			m_SampleRate = sampleRate;
			m_ChannelCount = channelCount;

			m_Recorder.registerSignalReceiver(this, VOICE_CHAT_SIGNAL_SAMPLES_READY);
			m_Recorder.initialize(m_SampleRate, m_ChannelCount);

			m_Playback.registerSignalReceiver(this, 0);
			m_Playback.initialize(m_SampleRate, m_ChannelCount);
			m_Playback.setSilenceDuration(m_PlaybackDelay);

			if (uint8_t err = m_Encoder.initialize(sampleRate, channelCount, m_Recorder.getSampleCountByCaptureFrequency()))
			{
				sendError(err);
			}

			if (uint8_t err = m_Decoder.initialize())
			{
				sendError(err);
			}
			m_Playback.start();
			return true;
		}

		void destroy()
		{
			stopRecording();
			m_Recorder.destroy();
			m_Playback.destroy();
			m_Encoder.destroy();
			m_Decoder.destroy();
		}

		bool startRecording()
		{
			return m_Recorder.start();
		}

		void stopRecording()
		{
			m_Recorder.stop();
		}

		bool isReady() const
		{
			return m_Recorder.isInitialized();
		}

		uint32_t getSampleRate() const
		{
			return m_SampleRate;
		}

		uint8_t getChannelCount() const
		{
			return m_ChannelCount;
		}

		void setVolume(const std::string& name, float volume)
		{
			m_Playback.setSpeakerVolume(name, volume);
		}

		void setCaptureFrequency(uint32_t milliseconds)
		{
			m_Recorder.setCaptureFrequency(milliseconds);
		}

		void setPlaybackDelay(uint32_t milliseconds)
		{
			m_Playback.setSilenceDuration(milliseconds);
		}

		void addSamples(const std::string& name, const SAMPLE_TYPE* samples, uint32_t sampleCount)
		{
			m_Playback.addSamples(name, samples, sampleCount);
		}

		void onSignal(uint8_t signal) override final
		{
			if (signal == VOICE_CHAT_SIGNAL_SAMPLES_READY)
			{
				static std::vector<SAMPLE_TYPE> samples;
				bool success = m_Recorder.getSamples(samples, m_Recorder.getSampleCountByCaptureFrequency());
				if (success && m_Parent)
				{
					m_Parent->onCaptureSamples(samples.data(), samples.size());
				}
			}
		}

		void onError(uint8_t error) override final
		{
			sendError(error);
		}

		const std::vector<char>& encode(const SAMPLE_TYPE* samples, uint32_t sampleCount)
		{
			if (uint8_t err = m_Encoder.initialize(m_SampleRate, m_ChannelCount, m_Recorder.getSampleCountByCaptureFrequency()))
			{
				sendError(err);
			}
			if (uint8_t err = m_Encoder.execute(samples, sampleCount))
			{
				sendError(err);
			}
			m_Encoder.finish();
			return m_Encoder.getData();
		}

		const std::vector<SAMPLE_TYPE>& decode(const char* samples, uint32_t size)
		{
			if (uint8_t err = m_Decoder.initialize())
			{
				sendError(err);
			}
			if (uint8_t err = m_Decoder.execute(samples, size))
			{
				sendError(err);
			}
			m_Decoder.finish();
			return m_Decoder.getData();
		}

		float calculateSoundPressureLevel(const SAMPLE_TYPE* samples, size_t sampleCount) const
		{
			float rms = calculateRootMeanSquare(samples, sampleCount);
			float normalizedRMS = rms / 32768.0f;  // 16-bit audio

			if (normalizedRMS == 0)
				return 0;  // Silence

			return 20.0f * std::log10(normalizedRMS / 20.0e-6f);  // SPL formula with reference pressure
		}

		void normalizeAmplitude(SAMPLE_TYPE* samples, size_t sampleCount, float currentSoundPressure, float targetSoundPressure)
		{
			float normalizationFactor = calculateNormalizationFactor(currentSoundPressure, targetSoundPressure);
			/*
			if (targetSoundPressure > currentSoundPressure)
			{
				float maxFactor = getHighestAllowedNormalizationFactor(samples, sampleCount);
				normalizationFactor = std::min<float>(normalizationFactor, maxFactor);
			}
			*/
			for (size_t i = 0; i < sampleCount; ++i)
			{
				samples[i] = static_cast<SAMPLE_TYPE>(samples[i] * normalizationFactor);
			}
		}

	private:
		void sendError(uint8_t error)
		{
			if (m_Parent)
			{
				m_Parent->onError(error);
			}
		}

		float calculateRootMeanSquare(const SAMPLE_TYPE* samples, size_t sampleCount) const
		{
			float sum = 0.0f;
			for (size_t i = 0; i < sampleCount; ++i)
			{
				sum += samples[i] * samples[i];
			}
			float mean = sum / sampleCount;
			return std::sqrt(mean);
		}

		float getHighestAllowedNormalizationFactor(const SAMPLE_TYPE* samples, size_t sampleCount)
		{
			int16_t highestAmplitude = 0;
			for (size_t i = 0; i < sampleCount; ++i)
			{
				int16_t current = abs(samples[i]);
				if (current > highestAmplitude)
					highestAmplitude = current;
			}

			float highestFactor = static_cast<float>(std::numeric_limits<int16_t>::max()) / highestAmplitude;
			return highestFactor;
		}

		float calculateNormalizationFactor(float currentSPL, float targetSPL) const
		{
			if (currentSPL <= 0)
				return 1.0f;

			return targetSPL / currentSPL;
		}

		void normalizeAmplitudeByFactor(SAMPLE_TYPE* samples, size_t sampleCount, float normalizationFactor)
		{
			for (size_t i = 0; i < sampleCount; ++i)
			{
				samples[i] = static_cast<SAMPLE_TYPE>(samples[i] * normalizationFactor);
			}
		}

	private:
		VoiceChat* m_Parent;
		uint32_t m_SampleRate;
		uint8_t m_ChannelCount;

		Recorder m_Recorder;
		Playback m_Playback;

		FLACEncoder m_Encoder;
		FLACDecoder m_Decoder;

		uint32_t m_PlaybackDelay;
		float m_TargetSpl;
	};
}

