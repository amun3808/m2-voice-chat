#pragma once
#include "all.h"

//#define MAX_OUT
namespace amun
{
#undef min
	class FLACEncoder : public FLAC::Encoder::Stream
	{
	public:
		virtual ~FLACEncoder()
		{
			destroy();
		}

		uint8_t initialize(uint32_t sampleRate, uint8_t channelCount, uint32_t blockSize)
		{
			set_sample_rate(sampleRate);
			set_channels(channelCount);
			set_bits_per_sample(8 * sizeof(SAMPLE_TYPE));
			// NOTE: 4608 is the maximum block size for streamable objects. It works up to 46k freq
			set_blocksize(blockSize / channelCount); // blockSize is the sampleCount based on capture frequency
#ifdef MAX_OUT
			set_compression_level(8); // 0-8
			set_do_mid_side_stereo(true);
			set_do_exhaustive_model_search(true);
			set_max_lpc_order(12);
			set_min_residual_partition_order(2);
			set_max_residual_partition_order(8);
#endif
			if (get_state() == FLAC__STREAM_ENCODER_UNINITIALIZED)
			{
				auto res = init();
				if (res != FLAC__STREAM_ENCODER_INIT_STATUS_OK)
				{
					return VOICE_CHAT_ERROR_FAILED_TO_INIT_ENCODER;
				}
			}
			return 0;
		}

		void destroy()
		{
			finish();
			m_EncodedData.clear();
			m_FlacSamples.clear();
		}

		uint8_t execute(const SAMPLE_TYPE* samples, uint32_t sampleCount)
		{
			m_EncodedData.clear();

			if (m_FlacSamples.size() < sampleCount)
				m_FlacSamples.resize(sampleCount);

			for (uint32_t i = 0; i < sampleCount; ++i)
			{
				m_FlacSamples[i] = samples[i];
			}

			uint32_t blockSize = get_blocksize();
			for (uint32_t offset = 0; offset < sampleCount; offset += blockSize * get_channels())
			{
				uint32_t blockSampleCount = std::min<uint32_t>(blockSize * get_channels(), sampleCount - offset);
				//trace("sampleCount %u, blocksize %u, blockSampleCount %u", sampleCount, blockSize, blockSampleCount);
				if (!process_interleaved(m_FlacSamples.data(), blockSampleCount / get_channels()))
				{
					finish();
					return VOICE_CHAT_ERROR_FAILED_TO_ENCODE;
				}
			}

			return 0;
		}


		FLAC__StreamEncoderWriteStatus write_callback(const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame) override
		{
			m_EncodedData.insert(m_EncodedData.end(), buffer, buffer + bytes);
			return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
		}

		FLAC__StreamEncoderSeekStatus seek_callback(FLAC__uint64) override
		{
			return FLAC__STREAM_ENCODER_SEEK_STATUS_UNSUPPORTED;
		}

		FLAC__StreamEncoderTellStatus tell_callback(FLAC__uint64*) override
		{
			return FLAC__STREAM_ENCODER_TELL_STATUS_UNSUPPORTED;
		}

		const std::vector<char>& getData() const
		{
			return m_EncodedData;
		}

	private:
		std::vector<char> m_EncodedData;
		std::vector<FLAC__int32> m_FlacSamples;
	};
}
