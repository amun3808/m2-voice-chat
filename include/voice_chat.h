// Public
#pragma once
#include <stdint.h>
#include <string>
#include "voice_chat_errors.h"

// Use static lib
#define VOICE_CHAT__STATIC_LIB

#ifdef VOICE_CHAT__STATIC_LIB
#define __VC_API__
#else // VOICE_CHAT__STATIC_LIB
#ifdef BUILDING_VOICE_CHAT
#define __VC_API__ __declspec(dllexport)
#else // BUILDING_VOICE_CHAT
#define __VC_API__ __declspec(dllimport)
#endif // BUILDING_VOICE_CHAT
#endif // VOICE_CHAT__STATIC_LIB

#ifdef _WIN64
#define __VC_PLATFORM__ "x64"
#elif defined(_WIN32)
#define __VC_PLATFORM__ "Win32"
#endif

#ifdef _DEBUG
#define __VC_CONFIG__ "Debug"
#else
#define __VC_CONFIG__ "Release"
#endif
#define VOICE_CHAT_LIB_NAME "voice_chat_core_" __VC_CONFIG__ "_" __VC_PLATFORM__


namespace amun
{
	class VoiceChatImpl;

	class __VC_API__ VoiceChat
	{
	public:
		VoiceChat();
		virtual ~VoiceChat();

		// default = 44100 samples, 2 channels. If you're using encoding, see the `encode()` note.
		bool init(uint32_t sampleRate = 44100, uint8_t channelCount = 2);

		// Stops & destroys the recorder and all the speakers.
		// init() can be called after destroy() if you want to reset it.
		void destroy();

		// Use to find out if you're ready to record or not
		bool isReady() const;

		uint32_t getSampleRate() const;
		uint8_t getChannelCount() const;

		bool startRecording();
		void stopRecording();

		// default = 100ms, min = 10ms.
		// Determines how soon you receive the microphone samples.
		void setCaptureFrequency(uint32_t milliseconds);

		// default = 250ms. Acts as a jitter buffer.
		// Don't forget to take possible network delays into consideration when changing this.
		void setPlaybackDelay(uint32_t milliseconds);

		// default = 100, min = 0, max = 100
		void setVolume(const std::string& speakerID, float volume = 100.0f);
		void addSamples(const std::string& speakerID, const int16_t* samples, uint32_t sampleCount);

		// The encoding is limited to up to 46k freq, after which you'll start hearing breakups
		const std::vector<char>& encode(const int16_t* samples, uint32_t sampleCount);
		const std::vector<int16_t>& decode(const char* samples, uint32_t size);

		float calculateSoundPressureLevel(const int16_t* samples, size_t sampleCount) const;
		void normalizeAmplitude(int16_t* samples, uint32_t sampleCount, float currentSoundPressure, float targetSoundPressure);

	protected:
		virtual void onCaptureSamples(const int16_t* samples, uint32_t sampleCount) = 0;
		virtual void onError(uint8_t error) = 0;

	private:
		VoiceChatImpl* m_Impl;
		friend class VoiceChatImpl;
	};
}

