// find
	void			EffectPacket(int32_t enumEffectType);

// change
	void			EffectPacket(int32_t enumEffectType, LPDESC specificDesc = nullptr);

// Add at the end, before };
#ifdef ENABLE_VOICE_CHAT
public:
	void VoiceChatSetConfig(uint8_t type, bool flag);
	bool VoiceChatIsBlock(uint8_t type) const;
	int VoiceChatGetConfig() const;
#endif
