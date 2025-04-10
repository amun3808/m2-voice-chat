# Find
class Window(object):

#Find
	def __init__(self, layer = "UI"):

# Add
		self.overFunc = None
		self.overArgs = None
		self.overOutFunc = None
		self.overOutArgs = None	

#Find
	def __del__(self):

# Add after wndMgr.Destroy(self.hWnd)
		self.overFunc = None
		self.overArgs = None
		self.overOutFunc = None
		self.overOutArgs = None

# Find
	def RegisterWindow(self, layer):

# Add above
	def SAFE_SetOverInEvent(self, func, *args):
		self.overFunc = __mem_func__(func)
		self.overArgs = args

	def SAFE_SetOverOutEvent(self, func, *args):
		self.overOutFunc = __mem_func__(func)
		self.overOutArgs = args
