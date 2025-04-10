#add
from _weakref import proxy


# in def __init__
#find
		self.xBar = 0

# add
		if app.ENABLE_VOICE_CHAT:
			self.interface = None

# Find
		btnChatSizing = self.ChatButton()
		btnChatSizing.SetOwner(self)
		btnChatSizing.SetMoveEvent(ui.__mem_func__(self.Refresh))
		btnChatSizing.Hide()
		self.btnChatSizing = btnChatSizing

# Add after
		if app.ENABLE_VOICE_CHAT:
			voiceChatButton = ui.Button()
			voiceChatButton.SetParent(self.btnChatSizing)
			voiceChatButton.SetUpVisual("d:/ymir work/ui/chat/btn_addtab01_default.tga")
			voiceChatButton.SetOverVisual("d:/ymir work/ui/chat/btn_addtab01_over.tga")
			voiceChatButton.SetDownVisual("d:/ymir work/ui/chat/btn_addtab01_down.tga")
			voiceChatButton.SAFE_SetEvent(self.__ToggleVoiceChat)
			voiceChatButton.SetToolTipText(localeInfo.CHAT_VOICE_CHAT)
			voiceChatButton.Hide()
			self.voiceChatButton = voiceChatButton


# Find
	def Destroy(self):
		self.chatInputSet.Destroy()

# Add
		if app.ENABLE_VOICE_CHAT:
			self.voiceChatButton = None

# Find
		self.chatInputSet.Open()

# Add above
		if app.ENABLE_VOICE_CHAT:
			self.voiceChatButton.Show()


# Find
		self.btnChatSizing.Hide()

# Add
		if app.ENABLE_VOICE_CHAT:
			self.voiceChatButton.Hide()



# In def __RefreshSizingBar(self):

# Find
		(x, y, width, height) = self.GetRect()


# add
		if app.ENABLE_VOICE_CHAT:
			self.voiceChatButton.SetPosition(width - 23, 3)


# Find
	def BindInterface(self, interface):

# Add
		if app.ENABLE_VOICE_CHAT:
			self.interface = proxy(interface)


# Find
	def BindInterface(self, interface):

# Add above
	if app.ENABLE_VOICE_CHAT:
		def __ToggleVoiceChat(self):
			if self.interface:
				self.interface.ToggleVoiceChatConfigWindow()
