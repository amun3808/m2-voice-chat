// find
	int		m_recvBufSize;
	int		m_recvBufInputPos;
	int		m_recvBufOutputPos;


// replace
#ifdef ENABLE_VOICE_CHAT
protected:
#endif
	int		m_recvBufSize;
	int		m_recvBufInputPos;
	int		m_recvBufOutputPos;
#ifdef ENABLE_VOICE_CHAT
private:
#endif



// find
bool Send(int len, const void* pSrcBuf);

// add
bool HasSpaceToSend(int size);

