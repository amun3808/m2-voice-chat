# Find
		net.SetTCPSendBufferSize(4096)


# Change
		if app.ENABLE_VOICE_CHAT:
			net.SetTCPSendBufferSize(1024 * 12) # 12kb
		else:
			net.SetTCPSendBufferSize(4096)

