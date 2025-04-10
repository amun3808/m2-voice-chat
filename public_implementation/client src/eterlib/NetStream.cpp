// in bool CNetworkStream::__RecvInternalBuffer()

// Find
	if (m_recvBufOutputPos > 0)
	{
		int recvBufDataSize = m_recvBufInputPos - m_recvBufOutputPos;
		if (recvBufDataSize > 0)
		{
			memmove(m_recvBuf, m_recvBuf + m_recvBufOutputPos, recvBufDataSize);
		}

		m_recvBufInputPos -= m_recvBufOutputPos;
		m_recvBufOutputPos = 0;
	}


// Replace
	if (m_recvBufOutputPos > 0)
	{
		if (m_recvBufInputPos > m_recvBufOutputPos)
		{
			int recvBufDataSize = m_recvBufInputPos - m_recvBufOutputPos;
			memmove(m_recvBuf, m_recvBuf + m_recvBufOutputPos, recvBufDataSize);
			m_recvBufInputPos -= m_recvBufOutputPos;
		}
		else
		{
			m_recvBufInputPos = 0;
		}

		m_recvBufOutputPos = 0;
	}



// find
bool CNetworkStream::Send(int len, const void* pSrcBuf)
{
	return Send(len, (const char*)pSrcBuf);
}

// add
bool CNetworkStream::HasSpaceToSend(int size)
{
	int sendBufRestSize = m_sendBufSize - m_sendBufInputPos;
	return sendBufRestSize >= size;
}
