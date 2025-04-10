#pragma once
#ifdef ENABLE_VOICE_CHAT
#include "Packet.h"
#include <voice/voice_chat.h>
#include <voice/voice_chat_errors.h>

class VoiceChat : public amun::VoiceChat, public CSingleton<VoiceChat>
{
public:
	VoiceChat();
	virtual ~VoiceChat();

	void Initialize();
	void Destroy();

	bool IsRecording() const;
	bool StartRecording();
	void StopRecording();

	uint32_t GetSampleRate() const;
	uint8_t GetChannelCount() const;

	void SetChatType(uint8_t chatType);
	void SetRecorderState(uint8_t newRecorderState);
	void StartTalking();
	void StopTalking();

	void SetDisabled(bool flag);

	void SetVolume(float volume);
	void SetSpeakerVolume(const std::string& name, float volume);

	void OnRecvVoiceChatPacket(const TVoiceChatPacket& packet, const char* data);
	void Process();

	void onCaptureSamples(const int16_t* samples, uint32_t sampleCount) override final;
	void onError(uint8_t error) override final;

private:
	bool processVoice(int16_t* samples, std::size_t sampleCount);

private:
	uint8_t m_ChatType;
	uint8_t m_RecorderState;
	bool m_Disabled;
	bool m_IsRecording;
	bool m_IsInitialized;
	float m_Volume;
	std::unordered_map<std::string, float> m_SpeakerVolumes;
	// Don't foget to hash these if you use any kind of hashing for client strings.
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
#endif
