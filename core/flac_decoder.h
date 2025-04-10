#pragma once
#include "all.h"

namespace amun
{
	class FLACDecoder : public FLAC::Decoder::Stream
	{
	public:
		virtual ~FLACDecoder()
		{
			destroy();
		}

		uint8_t initialize()
		{
			if (get_state() == FLAC__STREAM_DECODER_UNINITIALIZED && init() != FLAC__STREAM_DECODER_INIT_STATUS_OK)
			{
				return VOICE_CHAT_ERROR_FAILED_TO_INIT_DECODER;
			}
			return 0;
		}

		void destroy()
		{
			m_DecodedData.clear();
			finish();
		}

		uint8_t execute(const char* data, size_t size)
		{
			m_DecodedData.clear();
			m_DataReadOffset = 0;
			m_DataSize = size;
			m_InputData = data;

			if (size && !process_single())
			{
				finish();
				return VOICE_CHAT_ERROR_FAILED_TO_DECODE;
			}

			return 0;
		}

		const std::vector<FLAC__int16>& getData()
		{
			return m_DecodedData;
		}

	protected:
		FLAC__StreamDecoderReadStatus read_callback(FLAC__byte buffer[], size_t* bytes) override
		{
			if (*bytes)
			{
				if (m_DataReadOffset >= m_DataSize)
				{
					*bytes = 0;
					return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
				}

				size_t bytesToRead = std::min(*bytes, m_DataSize - m_DataReadOffset);
				std::memcpy(buffer, m_InputData + m_DataReadOffset, bytesToRead);
				m_DataReadOffset += bytesToRead;
				*bytes = bytesToRead;
			}
			return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
		}

		FLAC__StreamDecoderWriteStatus write_callback(const FLAC__Frame* frame, const FLAC__int32* const buffer[]) override
		{
			size_t numSamples = frame->header.blocksize;
			size_t numChannels = frame->header.channels;

			for (size_t i = 0; i < numSamples; ++i)
			{
				for (size_t ch = 0; ch < numChannels; ++ch)
				{
					m_DecodedData.push_back(static_cast<FLAC__int16>(buffer[ch][i]));
				}
			}
			return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
		}

		FLAC__StreamDecoderSeekStatus seek_callback(FLAC__uint64) override
		{
			return FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED;
		}

		FLAC__StreamDecoderTellStatus tell_callback(FLAC__uint64*) override
		{
			return FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED;
		}

		void error_callback(FLAC__StreamDecoderErrorStatus status) override
		{
			//std::cout << "Decoding error: " << FLAC__StreamDecoderErrorStatusString[status] << std::endl;
		}

	private:
		std::vector<FLAC__int16> m_DecodedData;
		const char* m_InputData{ nullptr };
		size_t m_DataSize{ 0 };
		size_t m_DataReadOffset{ 0 };
	};
}

