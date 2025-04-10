// in void CInputLogin::Entergame(LPDESC d, const char * data)

// find
	_send_bonus_info(ch);


//add
#ifdef ENABLE_VOICE_CHAT
	ch->ChatPacket(CHAT_TYPE_COMMAND, "voice_chat_disabled %u", g_bVoiceChatDisabled);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "voice_chat_config %d", ch->VoiceChatGetConfig());
#endif


// find
		case HEADER_CG_GUILD_SYMBOL_CRC:
			GuildSymbolCRC(d, c_pData);
			break;


// add
#ifdef ENABLE_VOICE_CHAT
		case HEADER_CG_VOICE_CHAT:
		{
			TVoiceChatPacket* p = (TVoiceChatPacket*)c_pData;
			return p->dataSize;
		}
#endif

