// Add
#ifdef ENABLE_VOICE_CHAT
#include "VoiceChat.h"
#endif

// Find
	m_kAccountConnector.Process();

// Add
#ifdef ENABLE_VOICE_CHAT
	m_VoiceChat.Process();
#endif
