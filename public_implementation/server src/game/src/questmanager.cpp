// in void CQuestManager::SetEventFlag(const string& name, int value)

// find
		else if (name == "new_xmas_event")
		{
			....
		}
// add above(or after, whatever)
#ifdef ENABLE_VOICE_CHAT
		else if (name == "voice_chat_disable")
		{
			bool disabled = (bool)value;
			if (g_bVoiceChatDisabled == disabled)
				return;

			g_bVoiceChatDisabled = disabled;
		}
#endif
