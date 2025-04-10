// find
	Set(HEADER_CG_STATE_CHECKER, sizeof(BYTE), "ServerStateCheck");


// add
#ifdef ENABLE_VOICE_CHAT
	Set(HEADER_CG_VOICE_CHAT, sizeof(TVoiceChatPacket), "VoiceChatPacket");
#endif
