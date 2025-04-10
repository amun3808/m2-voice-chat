// add
#ifdef ENABLE_VOICE_CHAT
#include "VoiceChat.h"
#endif


// find
			default:
				ret = RecvDefaultPacket(header);
				break;


// add above
#ifdef ENABLE_VOICE_CHAT
			case HEADER_GC_VOICE_CHAT:
				ret = RecvVoiceChatPacket();
				break;
#endif



// find
void CPythonNetworkStream::__LeaveGamePhase()
{
	CInstanceBase::ClearPVPKeySystem();

	__ClearNetworkActorManager();

	m_bComboSkillFlag = FALSE;



// add
#if defined(ENABLE_VOICE_CHAT)
	VoiceChat::Instance().Destroy();
#endif



// find
void CPythonNetworkStream::SetGamePhase()
{
	if ("Game"!=m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Game Phase ##");
	Tracen("");

	m_strPhase = "Game";

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::GamePhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveGamePhase);


// add
#if defined(ENABLE_VOICE_CHAT)
	VoiceChat::Instance().Initialize();
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_ReloadVoiceChatVolumes", Py_BuildValue("()"));
#endif




// Add at the end
#if defined(ENABLE_VOICE_CHAT)
bool CPythonNetworkStream::RecvVoiceChatPacket()
{
	static auto& rkVoice = VoiceChat::Instance();
	TVoiceChatPacket packet{};

	if (!Recv(sizeof(packet), &packet))
	{
		TraceError("FAILED TO RECEIVE VOICE CHAT PACKET");
		return false;
	}

	const uint32_t bufSize = sizeof(int16_t) * std::max<uint32_t>(4800, rkVoice.GetSampleRate()) * std::max<uint8_t>(8, rkVoice.GetChannelCount());
	static std::vector<char> buf(bufSize, 0/*silence*/); // Default to enough for 48k frequency on 8 channels
	if (buf.size() < bufSize)
		buf.resize(bufSize); // Tf are you doing to fill this?

	if (!Recv(packet.dataSize, buf.data()))
	{
		TraceError("FAILED TO RECEIVE AUDIO DATA! size %u, vid %u, name %s, totalSize %u, recvBufSize %d(out %d, in %d, max %d)",
			packet.dataSize, packet.vid, packet.name, packet.size, GetRecvBufferSize(), m_recvBufOutputPos, m_recvBufInputPos, m_recvBufSize);
		return false;
	}

	if (auto inst = CPythonCharacterManager::Instance().GetInstancePtr(packet.vid))
	{
		inst->AttachSpecialEffect(CInstanceBase::EFFECT_VOICE_CHAT);
	}

	rkVoice.OnRecvVoiceChatPacket(packet, buf.data());
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OnVoice",
		Py_BuildValue("(si)", packet.name, packet.type));

	return true;
}
#endif
