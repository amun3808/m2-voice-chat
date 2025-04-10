// find
int CInputMain::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)


// add above
#if defined(ENABLE_VOICE_CHAT)
struct FuncSendVoiceChatPacket
{
	FuncSendVoiceChatPacket(LPCHARACTER ch, const TVoiceChatPacket& packet, const char* audioData)
	{
		m_Character = ch;
		m_Packet = packet;
		m_AudioData = audioData;
		m_PlayerCount = 0;
	}

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			LPDESC d = ch->GetDesc();

			if (!d)
				return;

			if (ch == m_Character)
			{
				if (!test_server || quest::CQuestManager::Instance().GetEventFlag("voice_chat_hear_yourself") == 0)
					return;
			}

			if (ch->VoiceChatIsBlock(m_Packet.type))
				return;

			// If you have messenger block
			//if (BlocklistManager::instance().IsBlocked(m_Character->GetName(), ch->GetName()))
			//	return;

			//if (BlocklistManager::instance().IsBlocked(ch->GetName(), m_Character->GetName()))
			//	return;

			int distance = DISTANCE_APPROX(m_Character->GetX() - ch->GetX(), m_Character->GetY() - ch->GetY());
			if (m_Packet.type == VOICE_CHAT_TYPE_LOCAL && distance > 5'000)
				return;

			m_Packet.distance = static_cast<uint16_t>(distance);
			d->BufferedPacket(&m_Packet, sizeof(m_Packet));
			d->Packet(m_AudioData, m_Packet.dataSize);
			m_PlayerCount++;
		}
	}

	int m_PlayerCount;

private:
	LPCHARACTER m_Character;
	TVoiceChatPacket m_Packet;
	const char* m_AudioData;
};

int CInputMain::HandleVoiceChatPacket(const LPCHARACTER ch, const char* c_pData, size_t uiBytes)
{
	LPDESC d = ch->GetDesc();
	TVoiceChatPacket pack = *(TVoiceChatPacket*)c_pData;
	pack.header = HEADER_GC_VOICE_CHAT;
	pack.vid = ch->GetVID();
	strlcpy(pack.name, ch->GetName(), sizeof(pack.name));

	if (!pack.dataSize)
		return 0;

	if (pack.dataSize != pack.size - sizeof(TVoiceChatPacket))
		return -1;

	if (uiBytes < pack.size)
		return -1;

	if (!pack.type || pack.type >= VOICE_CHAT_TYPE_MAX_NUM || ch->FindAffect(AFFECT_BLOCK_CHAT))
		return pack.dataSize;

	if (ch->GetLevel() < VOICE_CHAT_MIN_LEVEL)
	{
		static std::unordered_map<uint32_t, int> toNotify;
		int& nextNotification = toNotify[ch->GetPlayerID()];
		if (nextNotification < thecore_pulse())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You need level %d to use the voice chat."), VOICE_CHAT_MIN_LEVEL);
			nextNotification = thecore_pulse() + PASSES_PER_SEC(1);
		}
		return pack.dataSize;
	}

	if (ch->VoiceChatIsBlock(pack.type))
		return pack.dataSize;

	if (g_bVoiceChatDisabled)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "voice_chat_disabled 1");
		return pack.dataSize;
	}

	FuncSendVoiceChatPacket f(ch, pack, c_pData + sizeof(pack));

	switch (pack.type)
	{
	case VOICE_CHAT_TYPE_LOCAL:
	{
		if (LPSECTREE tree = ch->GetSectree())
		{
			ch->EffectPacket(SE_VOICE_CHAT, d);
			tree->ForEachAround(f);
		}
	} break;

	case VOICE_CHAT_TYPE_PARTY:
	{
		if (LPPARTY party = ch->GetParty())
		{
			ch->EffectPacket(SE_VOICE_CHAT, d);
			party->ForEachOnMapMember(f, ch->GetMapIndex());
		}
	} break;

	case VOICE_CHAT_TYPE_GUILD:
	{
		if (CGuild* guild = ch->GetGuild())
		{
			ch->EffectPacket(SE_VOICE_CHAT, d);
			guild->ForEachOnMapMember(f, ch->GetMapIndex());
		}
	} break;

	default:
		sys_err("Unhandled voice type %u", pack.type);
		break;
	}

	return pack.dataSize;
}
#endif



// find in int CInputMain::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)

		case HEADER_CG_PONG:
			Pong(d);
			break;


// add
#ifdef ENABLE_VOICE_CHAT
		case HEADER_CG_VOICE_CHAT:
			if ((iExtraLen = HandleVoiceChatPacket(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;

			break;
#endif



