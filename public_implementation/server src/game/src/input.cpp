// If you're using a switch:
// find
int CInputHandshake::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)
{
	switch (bHeader)
	{


// add
#ifdef ENABLE_VOICE_CHAT
	case HEADER_CG_VOICE_CHAT:
	{
		TVoiceChatPacket* p = (TVoiceChatPacket*)c_pData;
		return p->dataSize;
	}
#endif




// If you're using if else
// In int CInputHandshake::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)

// Find
	else if (bHeader == HEADER_CG_PONG)


// add above
#ifdef ENABLE_VOICE_CHAT
	else if (bHeader ==  HEADER_CG_VOICE_CHAT)
	{
		TVoiceChatPacket* p = (TVoiceChatPacket*)c_pData;
		return p->dataSize;
	}
#endif
