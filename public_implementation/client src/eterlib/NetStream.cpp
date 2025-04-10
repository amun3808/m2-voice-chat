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
