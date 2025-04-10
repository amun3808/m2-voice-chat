// add at the end

#ifdef ENABLE_VOICE_CHAT
ACMD(do_voice_chat_config)
{
	char arg[256];
	one_argument(argument, arg, sizeof(arg));
	
	uint8_t type;
	str_to_number(type, arg);
	
	if (!type || type >= VOICE_CHAT_TYPE_MAX_NUM)
		return;
	
	ch->VoiceChatSetConfig(type, !ch->VoiceChatIsBlock(type));
}
#endif

