#include <iostream>

#include "voice_chat.h"
#pragma comment(lib, VOICE_CHAT_LIB_NAME)

#define USE_COMPRESSION

class MyVoiceChat : public amun::VoiceChat
{
public:
	void onCaptureSamples(const int16_t* samples, uint32_t sampleCount) override final
	{
		static std::vector<int16_t> samplesVec;
		samplesVec.clear();
		samplesVec.insert(samplesVec.end(), samples, samples + sampleCount);

		if (!processVoice(samplesVec.data(), samplesVec.size()))
		{
			std::cout << "Silence.." << std::endl;
			return;
		}
		else
		{
			std::cout << "Sound.." << std::endl;
		}

#ifdef USE_COMPRESSION
		// Test encoding
		const auto& encodedSamples = encode(samplesVec.data(), samplesVec.size());
		// Test decoding
		const auto& decodedSamples = decode(encodedSamples.data(), encodedSamples.size());
		addSamples("Amun", decodedSamples.data(), decodedSamples.size());
#else
		addSamples("Amun", samplesVec.data(), samplesVec.size());
#endif

	};

	void onError(uint8_t error) override final
	{
		std::cout << "VOICE CHAT ERROR: " << m_Errors[error] << std::endl;
	}

private:
	bool processVoice(int16_t* samples, std::size_t sampleCount)
	{
		static int inactiveBypassLeft = 0;
		static float smallestSplValue = 100;
		constexpr float targetSPL = 65.0f;  // normalize to 65 dB SPL

		float currentSPL = calculateSoundPressureLevel(samples, sampleCount);
		if (currentSPL < 10)
			return false;

		if (currentSPL < smallestSplValue)
		{
			smallestSplValue = currentSPL;
		}

		// Normalize to target SPL to make sure we're not too loud
		if (currentSPL > targetSPL)
		{
			normalizeAmplitude(samples, sampleCount, currentSPL, targetSPL);
		}

		float requiredBasedOnSmallest = std::fmax(20.0f, static_cast<float>(smallestSplValue * 1.5));
		bool ret = false;
		if (currentSPL > requiredBasedOnSmallest)
		{
			inactiveBypassLeft = 4;
			ret = true;
		}
		else if (--inactiveBypassLeft > 0)
		{
			ret = true;
		}

		return ret;
	}

	std::unordered_map<uint8_t, const char*> m_Errors{
		{ amun::VOICE_CHAT_ERROR_NONE, "" },
		{ amun::VOICE_CHAT_ERROR_FAILED_TO_INIT_CAPTURE_DEVICE, "Failed to initialize capture device." },
		{ amun::VOICE_CHAT_ERROR_FAILED_TO_START_CAPTURE_DEVICE, "Failed to start capture device." },
		{ amun::VOICE_CHAT_ERROR_FAILED_TO_INIT_PLAYBACK_DEVICE, "Failed to initialize playback device." },
		{ amun::VOICE_CHAT_ERROR_FAILED_TO_START_PLAYBACK_DEVICE, "Failed to start playback device." },
		{ amun::VOICE_CHAT_ERROR_FAILED_TO_INIT_ENCODER, "Failed to initialize encoder." },
		{ amun::VOICE_CHAT_ERROR_FAILED_TO_INIT_DECODER, "Failed to initialize decoder." },
		{ amun::VOICE_CHAT_ERROR_FAILED_TO_ENCODE, "Failed to encode samples." },
		{ amun::VOICE_CHAT_ERROR_FAILED_TO_DECODE, "Failed to decode samples." },
	};
};

int main()
{
	constexpr int sampleRate = 32000;
	constexpr int channelCount = 1;

	MyVoiceChat voice;
	//voice.setCaptureFrequency(100);
	voice.init(sampleRate, channelCount);
	voice.startRecording();

	bool isOn = true;
	while (1)
	{
		std::cin.get();
		if (isOn)
		{
			isOn = false;
			voice.stopRecording();
		}
		else
		{
			isOn = true;
			voice.startRecording();
		}
	}

	voice.stopRecording();
	voice.destroy();

	return 0;
}

