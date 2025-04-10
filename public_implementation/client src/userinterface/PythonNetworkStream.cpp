// Find
			Set(HEADER_GC_LOVER_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCLoverInfo), STATIC_SIZE_PACKET));

// add
#ifdef ENABLE_VOICE_CHAT
			Set(HEADER_GC_VOICE_CHAT, CNetworkPacketHeaderMap::TPacketType(sizeof(TVoiceChatPacket), DYNAMIC_SIZE_PACKET));
#endif
