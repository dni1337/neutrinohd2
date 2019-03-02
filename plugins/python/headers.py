from neutrino import CMenuTarget
from neutrino import CMessageBox, CHelpBox, CInfoBox, CHintBox
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
from neutrino import RC_nokey, RC_red, RC_green, RC_yellow, RC_blue, RC_home
from neutrino import CFileBrowser, CFileFilter
from neutrino import SNeutrinoSettings
from neutrino import button_label_struct
from neutrino import NONEXISTANT_LOCALE
from neutrino import NeutrinoMessages

import sys

## CMessageBox
def messageBox():
	msg = CMessageBox("neutrino: python", "first test\ntesting CMessageBox\ndas ist alles ;-)")
	msg._exec(-1)

## CHelpBox
def helpBox():
	hbox = CHelpBox()
	hbox.addLine("neutrino: python")
	hbox.addSeparator()
	hbox.addLine("first test")
	hbox.addPageBreak()
	hbox.addLine("testing CHelpBox\ndas ist alles ;-)")
	hbox.show("CHelpBox: python")

## CHeaders
def headers():
	CHeaders().paintHead(150, 50, 550, 40, "pythonTest", NEUTRINO_ICON_MOVIE)
	CHeaders().paintFoot(150, 570, 550, 40)

## CWindow
def window():
	CWindow(150,45,550,525).paint()

## CInfoBox
def infoBox():
	infoBox = CInfoBox("first test\ntesting CHintBox\ndas ist alles ;-)")
	infoBox.setText("first test\ntesting CHintBox\ndas ist alles ;-)")
	infoBox._exec()

## CFrameBuffer
def fb():
	frameBuffer = CFrameBuffer.getInstance()
	frameBuffer.paintBackground()

## ClistBox
def listBox():
	listbox = ClistBoxWidget("test", "mp3")
	listbox.enablePaintDate()
	listbox.addWidget(WIDGET_TYPE_CLASSIC)
	listbox.addWidget(WIDGET_TYPE_FRAME)
	listbox.enableWidgetChange()
	listbox._exec(None, "")
	listbox.hide()

## CMenuWidget
def menuWidget():
	menu = CMenuWidget("test", "mp3")
	menu.enableWidgetChange()
	menu.enablePaintFootInfo()

	item1 = CMenuForwarder("item1", True, "python-test", None, "red_action")
	item2 = CMenuForwarder("item2", True, "python-test", None, "green_action")
	item3 = CMenuForwarder("item3", True, "python-test", None, "yellow_action")
	item4 = CMenuForwarder("item4", True, "python-test", None, "blue_action")

	menu.addItem(item1)
	menu.addItem(item2)
	menu.addItem(item3)
	menu.addItem(item4)

	menu._exec(None, "")
	menu.hide()

## ClistBoxEntry
def listEntry():
	listboxEntry = ClistBox(150,10,550,615)
	listboxEntry.setTitle("test", "mp3")
	listboxEntry.enablePaintHead()
	listboxEntry.enablePaintDate()
	listboxEntry.enablePaintFoot()
	listboxEntry.paint()

## cPlayback
def playBack():
	playback = cPlayback(1)
	playback.Close()
	playback.Open()
	playback.Start("/home/mohousch/Music/AUD-20160209-WA0000.mp3")
	playback.Play()

## CAudioPlayerGui
def audioPlayerGui():
	aplay = CAudioPlayerGui()
	aplay.addToPlaylist('/home/mohousch/Music/AUD-20160209-WA0000.mp3')
	aplay._exec(None,"")

## CMoviePlayerGui
def moviePlayerGui():
	mplay=CMoviePlayerGui()
	mplay.addToPlaylist('/home/mohousch/Videos/ProSieben_20150619_201430.ts')
	mplay._exec(None,"")

## CPictureViewerGui
def pictureViewerGui():
	pic=CPictureViewerGui()
	pic.addToPlaylist("/home/mohousch/Pictures/funart.png")
	pic._exec(None,"")

## CPlugins
def plugins():
	plugins = CPlugins()
	plugins.startPlugin("nfilm")

def rcInput():
	rc = CRCInput()
	rc.messageLoop()

class main(CHeaders):
	def __init__(self):
		CHeaders.__init__(self)

		buttons = button_label_struct()
		buttons.button = NEUTRINO_ICON_BUTTON_HELP
		buttons.locale = NONEXISTANT_LOCALE
		buttons.localename = None

		fButtons = button_label_struct()
		fButtons.button = NEUTRINO_ICON_BUTTON_RED
		fButtons.locale = NONEXISTANT_LOCALE
		fButtons.localename = "red"
		
		self.enablePaintDate()
		self.enableLogo()

		self.setHeaderButtons(buttons, 1)

		self.paintHead(150, 50, 750, 40, "pythonTest", NEUTRINO_ICON_MOVIE)
		self.paintFoot(150, 570, 750, 40, 1, fButtons)

		CSwigHelpers().blit()

		SEC_TIMER_ID = 0
		SEC_TIMER_ID = CSwigHelpers().addTimer(1*1000*1000, False);

		msg = -1
		data = -1
		loop = True

		while loop:
			msg = CSwigHelpers().getRCCode()
			data = CSwigHelpers().getRCData()

			if msg == NeutrinoMessages().EVT_TIMER and data == SEC_TIMER_ID:
				self.paintHead(150, 50, 750, 40, "pythonTest", NEUTRINO_ICON_MOVIE)

			elif msg == RC_home:
				loop = False
				break

		CFrameBuffer.getInstance().paintBackground()

if __name__ == "__main__":
	main()










