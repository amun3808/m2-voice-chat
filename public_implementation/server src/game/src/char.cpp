// find
void CHARACTER::EffectPacket(int32_t enumEffectType)
{
	TPacketGCSpecialEffect p{};

	p.header = HEADER_GC_SPECIAL_EFFECT;
	p.type = enumEffectType;
	p.vid = GetVID();

	PacketAround(&p, sizeof(TPacketGCSpecialEffect));
}


// change
void CHARACTER::EffectPacket(int32_t enumEffectType, LPDESC specificDesc)
{
	TPacketGCSpecialEffect p{};

	p.header = HEADER_GC_SPECIAL_EFFECT;
	p.type = enumEffectType;
	p.vid = GetVID();

	if (specificDesc)
		specificDesc->Packet(&p, sizeof(TPacketGCSpecialEffect));
	else
		PacketAround(&p, sizeof(TPacketGCSpecialEffect));
}


// add at the end
#ifdef ENABLE_VOICE_CHAT
void CHARACTER::VoiceChatSetConfig(uint8_t type, bool flag)
{
	int questFlag = GetQuestFlag("voice_chat.config");
	if (flag)
	{
		SET_BIT(questFlag, (1 << type));
	}
	else
	{
		REMOVE_BIT(questFlag, (1 << type));
	}

	SetQuestFlag("voice_chat.config", questFlag);
	ChatPacket(CHAT_TYPE_COMMAND, "voice_chat_config %d", questFlag);
}

bool CHARACTER::VoiceChatIsBlock(uint8_t type) const
{
	int flag = GetQuestFlag("voice_chat.config");
	return IS_SET(flag, (1 << type));
}

int CHARACTER::VoiceChatGetConfig() const
{
	return GetQuestFlag("voice_chat.config");
}
#endif

