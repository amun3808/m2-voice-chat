// Change the header indexes if you have conflicts, but make sure they match the server

// Find
	HEADER_CG_STATE_CHECKER = 206,

// Add above
#ifdef ENABLE_VOICE_CHAT
	HEADER_CG_VOICE_CHAT = 207,
#endif


// Find
	HEADER_GC_SPECIFIC_EFFECT = 208,

// Add above
#ifdef ENABLE_VOICE_CHAT
	HEADER_GC_VOICE_CHAT = 182,
#endif

// Find
	SE_EQUIP_LOVE_PENDANT,		// ï¿½ß·ï¿½Å¸ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ò´ï¿½Æ®(71145) ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Æ® (ï¿½ßµï¿½ï¿½ï¿½ï¿½ï¿½Æ®ï¿½ï¿½, ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ® ï¿½Æ´ï¿½)

// Add at the end
#ifdef ENABLE_VOICE_CHAT
	SE_VOICE_CHAT,
#endif


// Find
#pragma pack(pop)


// Add above
#if defined(ENABLE_VOICE_CHAT)
enum EVoiceChatTypes : uint8_t
{
	VOICE_CHAT_TYPE_NONE,
	VOICE_CHAT_TYPE_LOCAL,
	VOICE_CHAT_TYPE_PARTY,
	VOICE_CHAT_TYPE_GUILD,
	VOICE_CHAT_TYPE_MAX_NUM,
};

typedef struct SVoiceChatPacket
{
	uint8_t header;
	uint16_t size;
	uint16_t dataSize;
	uint16_t distance;
	uint8_t type;
	bool isEncoded;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	uint32_t vid;
} TVoiceChatPacket;
#endif


