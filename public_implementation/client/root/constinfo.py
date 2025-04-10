# Add
if app.ENABLE_VOICE_CHAT:
	VOICE_CHAT_CONFIG = {}
	VOICE_CHAT_CONFIG_LOADED = False
	def LoadVoiceChatConfig():
		global VOICE_CHAT_CONFIG_LOADED
		if VOICE_CHAT_CONFIG_LOADED:
			return

		with open("voicechat.cfg", "a+") as f:
			lines = f.readlines()

		for line in lines:
			data = line.replace("\n", "").split("\t")
			if len(data) < 2:
				continue

			try:
				VOICE_CHAT_CONFIG[data[0]] = float(data[1])
			except:
				pass

		VOICE_CHAT_CONFIG_LOADED = True

	def SendVoiceChatVolumes():
		for playerName, vol in VOICE_CHAT_CONFIG.items():
			try:
				volume = float(vol)#In case we have anything that isn't a number
				app.SetVoiceChatVolumeForSpeaker(playerName, volume)
			except:
				pass

	def GetVoiceChatVolume(playerName):
		global VOICE_CHAT_CONFIG_LOADED
		if not VOICE_CHAT_CONFIG_LOADED:
			LoadVoiceChatConfig()

		if playerName in VOICE_CHAT_CONFIG:
			return VOICE_CHAT_CONFIG[playerName]

		return 1.0 # Default to 100

	def GetVoiceChatConfig(option):
		global VOICE_CHAT_CONFIG_LOADED
		if not VOICE_CHAT_CONFIG_LOADED:
			LoadVoiceChatConfig()

		if option in VOICE_CHAT_CONFIG:
			return int(VOICE_CHAT_CONFIG[option])

		return 0

	def SetVoiceChatConfig(option, value):
		VOICE_CHAT_CONFIG[option] = value
		# SaveVoiceChatConfig()

	def SetVoiceChatVolume(playerName, vol):
		VOICE_CHAT_CONFIG[playerName] = vol
		# SaveVoiceChatConfig()

	def SaveVoiceChatConfig():
		lines = []
		for playerName, vol in VOICE_CHAT_CONFIG.items():
			lines.append("{}\t{}\n".format(playerName, vol))

		with open("voicechat.cfg", "w+") as f:
			f.writelines(lines)
