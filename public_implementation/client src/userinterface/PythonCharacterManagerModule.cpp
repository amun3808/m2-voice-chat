// find
	PyModule_AddIntConstant(poModule, "EFFECT_LOVE_PENDANT_EQUIP",		CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP);


// add
#ifdef ENABLE_VOICE_CHAT
	PyModule_AddIntConstant(poModule, "EFFECT_VOICE_CHAT", CInstanceBase::EFFECT_VOICE_CHAT);
#endif

