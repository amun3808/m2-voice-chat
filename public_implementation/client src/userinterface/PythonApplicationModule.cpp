// find
void initapp()

// add above
#if defined(ENABLE_VOICE_CHAT)
PyObject* appVoiceChatSetChatType(PyObject* poSelf, PyObject* poArgs)
{
	uint8_t voiceChatType = 0;
	if (!PyTuple_GetByte(poArgs, 0, &voiceChatType))
		return Py_BuildException();

	VoiceChat::Instance().SetChatType(voiceChatType);
	return Py_BuildValue("i", 0);
}

PyObject* appVoiceChatStartTalking(PyObject* poSelf, PyObject* poArgs)
{
	VoiceChat::Instance().StartTalking();
	return Py_BuildNone();
}

PyObject* appVoiceChatStopTalking(PyObject* poSelf, PyObject* poArgs)
{
	VoiceChat::Instance().StopTalking();
	return Py_BuildNone();
}

PyObject* appVoiceChatSetDisabled(PyObject* poSelf, PyObject* poArgs)
{
	uint8_t disabled = 0;
	if (!PyTuple_GetByte(poArgs, 0, &disabled))
		return Py_BuildException();

	VoiceChat::Instance().SetDisabled(disabled);
	return Py_BuildNone();
}
PyObject* appVoiceChatSetVolumeForSpeaker(PyObject* poSelf, PyObject* poArgs)
{
	char* name;
	if (!PyTuple_GetString(poArgs, 0, &name))
		return Py_BuildException();

	float volume;
	if (!PyTuple_GetFloat(poArgs, 1, &volume))
		return Py_BuildException();

	VoiceChat::Instance().SetSpeakerVolume(name, static_cast<uint8_t>(volume * 100.0f));
	return Py_BuildNone();
}

PyObject* appVoiceChatSetRecorderState(PyObject* poSelf, PyObject* poArgs)
{
	uint8_t state;
	if (!PyTuple_GetInteger(poArgs, 0, &state))
		return Py_BuildException();

	VoiceChat::Instance().SetRecorderState(state);
	return Py_BuildNone();
}

PyObject* appVoiceChatSetVolume(PyObject* poSelf, PyObject* poArgs)
{
	float volume;
	if (!PyTuple_GetFloat(poArgs, 0, &volume))
		return Py_BuildException();

	VoiceChat::Instance().SetVolume(static_cast<uint8_t>(volume * 100.0f));
	return Py_BuildNone();
}
#endif

// Find
		{ "OnLogoOpen",					appLogoOpen,					METH_VARARGS },

// add
#if defined(ENABLE_VOICE_CHAT)
		{ "VoiceChatSetDisabled",			appVoiceChatSetDisabled,				METH_VARARGS },
		{ "VoiceChatStartTalking",			appVoiceChatStartTalking,				METH_VARARGS },
		{ "VoiceChatStopTalking",			appVoiceChatStopTalking,				METH_VARARGS },
		{ "VoiceChatSetChatType",			appVoiceChatSetChatType,					METH_VARARGS },
		{ "VoiceChatSetVolumeForSpeaker",	appVoiceChatSetVolumeForSpeaker,		METH_VARARGS },
		{ "VoiceChatSetRecorderState",		appVoiceChatSetRecorderState,				METH_VARARGS },
		{ "VoiceChatSetVolume",				appVoiceChatSetVolume,				METH_VARARGS },
#endif


// find
	PyModule_AddIntConstant(poModule, "CAMERA_STOP",			CPythonApplication::CAMERA_STOP);

// add
#ifdef ENABLE_VOICE_CHAT
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_NONE", VOICE_CHAT_TYPE_NONE);
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_LOCAL", VOICE_CHAT_TYPE_LOCAL);
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_PARTY", VOICE_CHAT_TYPE_PARTY);
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_GUILD", VOICE_CHAT_TYPE_GUILD);
	PyModule_AddIntConstant(poModule, "VOICE_CHAT_TYPE_MAX_NUM", VOICE_CHAT_TYPE_MAX_NUM);
#endif



// Add before }
#ifdef ENABLE_VOICE_CHAT
	PyModule_AddIntConstant(poModule, "ENABLE_VOICE_CHAT", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_VOICE_CHAT", 0);
#endif
