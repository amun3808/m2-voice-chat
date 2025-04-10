#pragma once
#include "all.h"

namespace amun
{
#undef max
#undef min

	class Recorder
	{
	public:
		Recorder() : m_Device({}), m_CaptureFrequency(100), m_Receiver(nullptr), m_ReceiverSignal(0)
		{}

		~Recorder()
		{
			destroy();
		}

		bool initialize(uint32_t sampleRate, uint8_t channelCount)
		{
			if (ma_device_get_state(&m_Device) != ma_device_state_uninitialized)
				return false;

			ma_device_config deviceConfig = ma_device_config_init(ma_device_type_capture);
			deviceConfig.capture.format = SAMPLE_FORMAT;
			deviceConfig.capture.channels = channelCount;
			deviceConfig.sampleRate = sampleRate;
			deviceConfig.dataCallback = dataCallback;
			deviceConfig.pUserData = this;

			if (ma_device_init(nullptr, &deviceConfig, &m_Device) != MA_SUCCESS)
			{
				notifyError(VOICE_CHAT_ERROR_FAILED_TO_INIT_CAPTURE_DEVICE);
				return false;
			}

			return true;
		}

		void destroy()
		{
			stop();
			ma_device_uninit(&m_Device);
			m_Samples.clear();
		}

		bool isInitialized() const
		{
			return ma_device_get_state(&m_Device) != ma_device_state_uninitialized;
		}

		bool start()
		{
			m_Samples.clear();
			if (ma_device_start(&m_Device) != MA_SUCCESS)
			{
				notifyError(VOICE_CHAT_ERROR_FAILED_TO_START_CAPTURE_DEVICE);
				ma_device_uninit(&m_Device);
				return false;
			}

			return true;
		}

		void stop()
		{
			ma_device_stop(&m_Device);
			m_Samples.clear();
		}

		void setCaptureFrequency(uint32_t milliseconds)
		{
			// Needs to be multiple of 10(easier to deal with the buffers)
			milliseconds = ((milliseconds + 5) / 10) * 10;
			m_CaptureFrequency = std::max(milliseconds, 10u);
		}

		uint32_t getSampleCountByCaptureFrequency() const
		{
			return std::max(m_CaptureFrequency, 10u) * samplesPerMillisecond();
		}

		void registerSignalReceiver(SignalReceiver* receiver, uint8_t signal = 0)
		{
			m_Receiver = receiver;
			m_ReceiverSignal = signal;
		}

		void unregisterSignalReceiver()
		{
			m_Receiver = nullptr;
			m_ReceiverSignal = 0;
		}

		bool getSamples(std::vector<SAMPLE_TYPE>& out, uint32_t sampleCount)
		{
			if (!m_Samples.empty())
			{
				out.assign(m_Samples.begin(), m_Samples.begin() + sampleCount);
				m_Samples.clear();
				return true;
			}
			return false;
			//if (m_Samples.size() < sampleCount)
			//	return false;

			//out.assign(m_Samples.begin(), m_Samples.begin() + sampleCount);
			//m_Samples.erase(m_Samples.begin(), m_Samples.begin() + sampleCount);
			//return true;
		}

	private:
		static void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
		{
			Recorder* recorder = (Recorder*)pDevice->pUserData;
			recorder->addSamples((const SAMPLE_TYPE*)pInput, frameCount * pDevice->capture.channels);
		}

		void addSamples(const SAMPLE_TYPE* pSamples, ma_uint32 sampleCount)
		{
			if (m_Receiver)
			{
				m_Samples.insert(m_Samples.end(), pSamples, pSamples + sampleCount);

				if (m_Samples.size() >= getSampleCountByCaptureFrequency())
				{
					m_Receiver->onSignal(m_ReceiverSignal);
				}
			}
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
		std::vector<SAMPLE_TYPE> m_Samples;
		uint32_t m_CaptureFrequency;
		SignalReceiver* m_Receiver;
		uint8_t m_ReceiverSignal;
	};
}