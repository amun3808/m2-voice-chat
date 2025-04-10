# Find
	def Close(self):
		self.Hide()

# Add
		if app.ENABLE_VOICE_CHAT:
			constInfo.SaveVoiceChatConfig()


# Find
		self.serverCommander=stringCommander.Analyzer()

# Add above
		if app.ENABLE_VOICE_CHAT:
			serverCommandList["voice_chat_disabled"] = self.__DisableVoiceChat
			serverCommandList["voice_chat_config"] = self.__ConfigVoiceChat


# Find
		onPressKeyDict[app.DIK_F]			= lambda : self.__PressFKey()


# Add
		onPressKeyDict[app.DIK_U]			= self.OnStartTalking


# Find
		onClickKeyDict[app.DIK_LSHIFT] = lambda: self.__SetQuickSlotMode()


# Add
		onClickKeyDict[app.DIK_U] = lambda: self.OnStopTalking()

# Add at the end
	if app.ENABLE_VOICE_CHAT:
		def BINARY_OnVoice(self, name, vcType = 0):
			if self.interface:
				if self.interface.wndVoiceChatConfig:
					self.interface.wndVoiceChatConfig.OnMessage(name, vcType)
				if self.interface.wndVoiceChatOverlay:
					self.interface.wndVoiceChatOverlay.OnMessage(name, vcType)

		def BINARY_ReloadVoiceChatVolumes(self):
			constInfo.SendVoiceChatVolumes()

		def OnStartTalking(self):
			if constInfo.GetVoiceChatConfig("VOICE_CHAT_MICROPHONE_STATE") != 1:
				if self.interface and self.interface.wndVoiceChatConfig and not self.interface.wndVoiceChatConfig.IsShow():
					self.interface.wndVoiceChatConfig.Show()
					if not self.interface.IsOpenChat():
						self.interface.ToggleChat()
					# self.interface.wndChat.voiceChatButton.Blink(1, True) # Enable if you have Blink or Flash
					return

			app.VoiceChatStartTalking()

		def OnStopTalking(self):
			app.VoiceChatStopTalking()

		def __DisableVoiceChat(self, disabled):
			disabled = int(disabled)
			app.VoiceChatSetDisabled(disabled)

		def __ConfigVoiceChat(self, arg):
			arg = int(arg)
			if self.interface and self.interface.wndVoiceChatConfig:
				self.interface.wndVoiceChatConfig.OnReceiveConfig(arg)
