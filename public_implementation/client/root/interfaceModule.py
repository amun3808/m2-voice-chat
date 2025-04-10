# Add
if app.ENABLE_VOICE_CHAT:
	import uiVoiceChatConfig


# Find
		self.wndGuildBuilding = None

# Add
		if app.ENABLE_VOICE_CHAT:
			self.wndVoiceChatConfig = None
			self.wndVoiceChatOverlay = None


# Find
		self.__MakeCubeWindow()

# Add
		if app.ENABLE_VOICE_CHAT:
			self.wndVoiceChatConfig = uiVoiceChatConfig.VoiceChatConfig()
			self.wndVoiceChatOverlay = uiVoiceChatConfig.SpeakerQueue()


# Find
		if self.wndGuildBuilding:
			self.wndGuildBuilding.Destroy()

# Add
		if app.ENABLE_VOICE_CHAT:
			if self.wndVoiceChatConfig:
				self.wndVoiceChatConfig.Hide()
				self.wndVoiceChatConfig.Destroy()
				del self.wndVoiceChatConfig

			if self.wndVoiceChatOverlay:
				self.wndVoiceChatOverlay.Hide()
				self.wndVoiceChatOverlay.Destroy()
				del self.wndVoiceChatOverlay





# Add at the end
	if app.ENABLE_VOICE_CHAT:
		def ToggleVoiceChatConfigWindow(self):
			if self.wndVoiceChatConfig:
				if self.wndVoiceChatConfig.IsShow():
					self.wndVoiceChatConfig.Hide()
				else:
					self.wndVoiceChatConfig.Show()
					self.wndVoiceChatConfig.SetTop()




















