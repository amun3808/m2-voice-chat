#define MINIAUDIO_IMPLEMENTATION
#include "all.h"
#include "voice_chat_impl.h"
#include "voice_chat.h"

#ifdef _DEBUG
#pragma comment(lib, "FLAC_d.lib")
#pragma comment(lib, "FLAC++_d.lib")
#else
#pragma comment(lib, "FLAC.lib")
#pragma comment(lib, "FLAC++.lib")
#endif

namespace amun
{
	VoiceChat::VoiceChat()
	{
		m_Impl = new VoiceChatImpl(this);
	}

	VoiceChat::~VoiceChat()
	{
		delete m_Impl;
		m_Impl = nullptr;
	}

	bool VoiceChat::init(uint32_t sampleRate, uint8_t channelCount)
	{
		return m_Impl->init(sampleRate, channelCount);
	}

	void VoiceChat::destroy()
	{
		m_Impl->destroy();
	}

	bool VoiceChat::startRecording()
	{
		return m_Impl->startRecording();
	}

	void VoiceChat::stopRecording()
	{
		m_Impl->stopRecording();
	}

	bool VoiceChat::isReady() const
	{
		return m_Impl->isReady();
	}

	uint32_t VoiceChat::getSampleRate() const
	{
		return m_Impl->getSampleRate();
	}

	uint8_t VoiceChat::getChannelCount() const
	{
		return m_Impl->getChannelCount();
	}

	void VoiceChat::setVolume(const std::string& name, float volume)
	{
		return m_Impl->setVolume(name, volume);
	}

	void VoiceChat::setCaptureFrequency(uint32_t milliseconds)
	{
		return m_Impl->setCaptureFrequency(milliseconds);
	}

	void VoiceChat::setPlaybackDelay(uint32_t milliseconds)
	{
		return m_Impl->setPlaybackDelay(milliseconds);
	}

	void VoiceChat::addSamples(const std::string& name, const SAMPLE_TYPE* samples, uint32_t sampleCount)
	{
		m_Impl->addSamples(name, samples, sampleCount);
	}

	const std::vector<char>& VoiceChat::encode(const SAMPLE_TYPE* samples, uint32_t sampleCount)
	{
		return m_Impl->encode(samples, sampleCount);
	}

	const std::vector<SAMPLE_TYPE>& VoiceChat::decode(const char* samples, uint32_t size)
	{
		return m_Impl->decode(samples, size);
	}

	float VoiceChat::calculateSoundPressureLevel(const SAMPLE_TYPE* samples, size_t sampleCount) const
	{
		return m_Impl->calculateSoundPressureLevel(samples, sampleCount);
	}

	void VoiceChat::normalizeAmplitude(int16_t* samples, uint32_t sampleCount, float currentSoundPressure, float targetSoundPressure)
	{
		m_Impl->normalizeAmplitude(samples, sampleCount, currentSoundPressure, targetSoundPressure);
	}
}
