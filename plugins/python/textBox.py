from neutrino import CMenuTarget
from neutrino import CMessageBox, CHelpBox, CInfoBox, CHintBox, CTextBox
from neutrino import CHeaders, CWindow 
from neutrino import ClistBoxWidget, CMenuWidget, CMenuForwarder, ClistBoxItem
from neutrino import ClistBox, ClistBoxEntryItem
from neutrino import CFrameBuffer, CRCInput
from neutrino import cPlayback
from neutrino import CAudioPlayerGui, CMoviePlayerGui, CPictureViewerGui
from neutrino import CPlugins
from neutrino import CBox, CFile, CSwigHelpers
from neutrino import DATADIR
from neutrino import WIDGET_TYPE_CLASSIC, WIDGET_TYPE_FRAME
from neutrino import NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MOVIE, NEUTRINO_ICON_PLUGIN, NEUTRINO_ICON_BUTTON_HELP
from neutrino import RC_nokey, RC_red, RC_green, RC_yellow, RC_blue, RC_home, RC_info
from neutrino import CFileBrowser, CFileFilter
from neutrino import SNeutrinoSettings
from neutrino import button_label_struct
from neutrino import NONEXISTANT_LOCALE
from neutrino import NeutrinoMessages

class main(CTextBox):
	msgText = "first test\ntesting CMessageBox\nthat's all Folk!"
	def __init__(self):
		CTextBox.__init__(self)
		self.setText(self.msgText)
		self.paint()
		CFrameBuffer.getInstance().blit()

		msg = -1
		loop = True
		bigFonts = False

		while loop:
			msg = CSwigHelpers().getRCCode()
			#data = CSwigHelpers().getRCData()

			if msg == RC_info:
				if bigFonts == False : 
					bigFonts = True
				elif bigFonts == True:
					bigFonts = False
				self.setBigFonts(bigFonts)
				self.refresh()

			if msg == RC_home:
				self.hide()
				CFrameBuffer.getInstance().blit()
				loop = False
				break

if __name__ == "__main__":
	main()










