#include "stdafx.h"
#ifdef ENABLE_VOICE_CHAT
#include "VoiceChat.h"
#include "PythonApplication.h"
#include "PythonNetworkStream.h"

#ifdef _DEBUG
static constexpr auto DISABLE_ACTIVE_WINDOW_CHECK_FOR_TALKING = true;
static constexpr auto DISABLE_ACTIVE_WINDOW_CHECK_FOR_HEARING = true;
#else
#define USE_COMPRESSION
static constexpr auto DISABLE_ACTIVE_WINDOW_CHECK_FOR_TALKING = false;
static constexpr auto DISABLE_ACTIVE_WINDOW_CHECK_FOR_HEARING = false;
#endif

static constexpr uint32_t SAMPLE_RATE = 32000;
static constexpr uint8_t CHANNEL_COUNT = 1;

enum EVoiceRecorderStates
{
	RECORDER_STATE_OFF,
	RECORDER_STATE_PUSH_TO_TALK,
	RECORDER_STATE_ALWAYS_ON,
};

VoiceChat::VoiceChat() : m_IsInitialized(false)
{
	Initialize();
}

VoiceChat::~VoiceChat()
{
	Destroy();
}

void VoiceChat::Initialize()
{
	if (m_IsInitialized)
		return;

	// Defaults
	m_ChatType = VOICE_CHAT_TYPE_LOCAL;
	m_RecorderState = RECORDER_STATE_OFF;
	m_Disabled = false;
	m_IsRecording = false;
	m_Volume = 100.0f;

	// Init
	amun::VoiceChat::setPlaybackDelay(50);
	amun::VoiceChat::init(SAMPLE_RATE, CHANNEL_COUNT);
	m_IsInitialized = true;
}

void VoiceChat::Destroy()
{
	StopRecording();
	amun::VoiceChat::destroy();
	m_IsInitialized = false;
}

bool VoiceChat::IsRecording() const
{
	return m_IsRecording;
}

bool VoiceChat::StartRecording()
{
	m_IsRecording = amun::VoiceChat::startRecording();
	return m_IsRecording;
}

void VoiceChat::StopRecording()
{
	amun::VoiceChat::stopRecording();
	m_IsRecording = false;
}

uint32_t VoiceChat::GetSampleRate() const
{
	return amun::VoiceChat::getSampleRate();
}

uint8_t VoiceChat::GetChannelCount() const
{
	return amun::VoiceChat::getChannelCount();
}

void VoiceChat::SetChatType(uint8_t chatType)
{
	m_ChatType = chatType;
}

void VoiceChat::SetRecorderState(uint8_t newRecorderState)
{
	if (m_RecorderState == RECORDER_STATE_ALWAYS_ON || newRecorderState == RECORDER_STATE_OFF)
	{
		StopRecording();
	}
	m_RecorderState = newRecorderState;
}

void VoiceChat::StartTalking()
{
	bool isActive = CPythonApplication::Instance().IsActive() || DISABLE_ACTIVE_WINDOW_CHECK_FOR_TALKING;
	if (!m_Disabled && isActive && m_RecorderState == RECORDER_STATE_PUSH_TO_TALK)
	{
		StartRecording();
	}
}

void VoiceChat::StopTalking()
{
	if (!m_Disabled && m_RecorderState == RECORDER_STATE_PUSH_TO_TALK)
	{
		StopRecording();
	}
}

void VoiceChat::SetDisabled(bool flag)
{
	m_Disabled = flag;
	if (m_Disabled && IsRecording())
	{
		StopRecording();
	}
}

void VoiceChat::SetVolume(float volume)
{
	m_Volume = volume;
}

void VoiceChat::SetSpeakerVolume(const std::string& name, float volume)
{
	m_SpeakerVolumes[name] = volume;
}

void VoiceChat::OnRecvVoiceChatPacket(const TVoiceChatPacket& packet, const char* data)
{
#undef min
	if (CPythonApplication::Instance().IsActive() || DISABLE_ACTIVE_WINDOW_CHECK_FOR_HEARING)
	{
		float volume = m_Volume * m_SpeakerVolumes[packet.name] / 100.0;
		if (packet.type == VOICE_CHAT_TYPE_LOCAL)
		{
			const float distanceToVolumeReduction = 100.0 - std::min(packet.distance / 100.0 * 1.9, 100.0); // 1.9 volume points per 100 distance
			volume = volume * distanceToVolumeReduction / 100;
		}

		amun::VoiceChat::setVolume(packet.name, volume);
		if (packet.isEncoded)
		{
			const auto& decodedSamples = amun::VoiceChat::decode(data, packet.dataSize);
			amun::VoiceChat::addSamples(packet.name, decodedSamples.data(), decodedSamples.size());
		}
		else
		{
			amun::VoiceChat::addSamples(packet.name, reinterpret_cast<const int16_t*>(data), packet.dataSize / sizeof(int16_t));
		}
	}
}

void VoiceChat::onCaptureSamples(const int16_t* samples, uint32_t sampleCount)
{
	if (m_Disabled)
		return;

	static auto& rkNet = CPythonNetworkStream::Instance();
	uint32_t dataSize = sampleCount * sizeof(int16_t); // This will always stay the same, unless we encode them.

	if (!samples || !sampleCount || (sampleCount > GetSampleRate() / 10)) // We're getting samples every 100ms
	{
		TraceError("Invalid samples. Count %u", sampleCount);
		return;
	}

	static std::vector<int16_t> samplesVec(sampleCount);
	samplesVec.clear();
	samplesVec.insert(samplesVec.end(), samples, samples + sampleCount);
	if (!processVoice(samplesVec.data(), samplesVec.size()))
		return;

#ifdef USE_COMPRESSION
	const auto& encodedSamples = amun::VoiceChat::encode(samplesVec.data(), samplesVec.size());
	dataSize = encodedSamples.size();
	if (!dataSize || dataSize > sampleCount * sizeof(int16_t))
	{
		TraceError("Invalid data size %u, sampleCount %u", dataSize, sampleCount);
		return;
	}
	//TraceError("Size before compression %u, after %u", samplesVec.size() * sizeof(int16_t), dataSize);
#endif

	TVoiceChatPacket packet{};
	packet.header = HEADER_CG_VOICE_CHAT;
	packet.type = m_ChatType;
	packet.dataSize = dataSize;
	packet.size = sizeof(packet) + dataSize;
#ifdef USE_COMPRESSION
	packet.isEncoded = true;
#endif
	if (rkNet.HasSpaceToSend(packet.size))
	{
		rkNet.Send(sizeof(TVoiceChatPacket), &packet);
#ifdef USE_COMPRESSION
		rkNet.Send(dataSize, encodedSamples.data());
#else
		rkNet.Send(dataSize, samplesVec.data());
#endif
	}
}

void VoiceChat::onError(uint8_t error)
{
	TraceError("VOICE CHAT ERROR: %s", m_Errors[error]);
}

bool VoiceChat::processVoice(int16_t* samples, std::size_t sampleCount)
{
	static int inactiveBypassLeft = 0;
	static float smallestSplValue = 100;
	constexpr float targetSPL = 65.0f;  // normalize to 65 dB SPL
	float currentSPL = amun::VoiceChat::calculateSoundPressureLevel(samples, sampleCount);

	if (currentSPL < 10)
		return false;

	if (currentSPL < smallestSplValue)
	{
		smallestSplValue = currentSPL;
	}

	// Normalize to target SPL to make sure we're not too loud
	if (currentSPL > targetSPL)
	{
		amun::VoiceChat::normalizeAmplitude(samples, sampleCount, currentSPL, targetSPL);
	}

	float requiredBasedOnSmallest = std::fmax(20.0f, static_cast<float>(smallestSplValue * 1.5));
	if (currentSPL > requiredBasedOnSmallest)
	{
		inactiveBypassLeft = 4;
		return true;
	}
	else if (--inactiveBypassLeft > 0)
	{
		return true;
	}

	return false;
}

void VoiceChat::Process()
{
	if (m_Disabled || !m_IsInitialized)
		return;

	bool isActive = CPythonApplication::Instance().IsActive() || DISABLE_ACTIVE_WINDOW_CHECK_FOR_TALKING;
	if (IsRecording() && !isActive)
	{
		StopRecording();
	}
	else if (!IsRecording() && isActive && m_RecorderState == RECORDER_STATE_ALWAYS_ON && amun::VoiceChat::isReady())
	{
		StartRecording();
	}
}
#endif
