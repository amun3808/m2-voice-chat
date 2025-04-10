// find
	HEADER_CG_HANDSHAKE = 255,


// add above
#ifdef ENABLE_VOICE_CHAT
	HEADER_CG_VOICE_CHAT = 207,
#endif


// find
	HEADER_GC_ROULETTE					= 200, //unused


// add above
#ifdef ENABLE_VOICE_CHAT
	HEADER_GC_VOICE_CHAT = 182,
#endif



// find
#pragma pack()

// add above
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


