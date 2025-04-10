// find
		void		Refine(LPCHARACTER ch, const char* c_pData);

// add
#ifdef ENABLE_VOICE_CHAT
	int HandleVoiceChatPacket(const LPCHARACTER ch, const char* c_pData, size_t uiBytes);
#endif
