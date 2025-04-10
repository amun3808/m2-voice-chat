// find
			TEMP_BUFFER	tempbuf;

// change(not needed if you're manually creating and deleting the buffer)
#ifdef ENABLE_VOICE_CHAT
			TEMP_BUFFER	tempbuf(MAX(8196, iSizeBuffer));
#else
			TEMP_BUFFER	tempbuf;
#endif


// change
void DESC::BufferedPacket(const void * c_pvData, int iSize)
{
	if (m_iPhase == PHASE_CLOSE)
		return;

#ifdef ENABLE_VOICE_CHAT
	if (!m_lpBufferedOutputBuffer)
		m_lpBufferedOutputBuffer = buffer_new(MAX(4096, iSize));
#else
	if (!m_lpBufferedOutputBuffer)
		m_lpBufferedOutputBuffer = buffer_new(MAX(1024, iSize));
#endif

	buffer_write(m_lpBufferedOutputBuffer, c_pvData, iSize);
}

