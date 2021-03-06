-- 
--

local selected = -1
local PATH = "/"

-- CMessageBox
function messageBox()
	title = "luaTest"
	msg = "TEST"
	mBox = neutrino.CMessageBox(title, msg)
	mBox:exec(-1)
end

-- CHelpBox
function helpBox()
	hbox = neutrino.CHelpBox()
	hbox:addLine("neutrino: lua")
	hbox:addSeparator()
	hbox:addLine("first test")
	hbox:addLine("testing CHelpBox\ndas ist alles ;-)")
	hbox:show("CHelpBox: lua")
end

-- CHintBox
function hintBox()
	hint = neutrino.CHintBox("neutrino: lua","first test\ntesting CHintBox\ndas ist alles ;-)")
	hint:exec()
end

-- CInfoBox
function infoBox()
	info = neutrino.CInfoBox()
	info:setText("first test\ntesting CHintBox\ndas ist alles ;-)")
	info:exec()
end

-- CStringInput
function stringInput()
	title = "luaTest: CStringInputSMS"
	local value
	input = neutrino.CStringInputSMS(title, vale)
	input:exec(null, "")
end

-- CAudioPlayerGui
function audioPlayer()
	fileBrowser = neutrino.CFileBrowser()
	fileFilter = neutrino.CFileFilter()
	fileFilter:addFilter("mp3")

	fileBrowser.Multi_Select = false
	fileBrowser.Dirs_Selectable = false
	fileBrowser.Filter = fileFilter

	fileBrowser:exec(PATH)

	PATH = fileBrowser:getCurrentDir()

	player = neutrino.CAudioPlayerGui()
	
	if fileBrowser:getSelectedFile() ~= null then
		player:addToPlaylist(fileBrowser:getSelectedFile())
		player:exec(None, "")
	end

	if fileBrowser:getExitPressed() ~= true then
		audioPlayer()
	end
end

-- CPictureViewerGui
function pictureViewer()
	fileBrowser = neutrino.CFileBrowser()
	fileFilter = neutrino.CFileFilter()
	fileFilter:addFilter("jpeg")
	fileFilter:addFilter("jpg")

	fileBrowser.Multi_Select = false
	fileBrowser.Dirs_Selectable = false
	fileBrowser.Filter = fileFilter

	fileBrowser:exec(PATH)

	PATH = fileBrowser:getCurrentDir()

	player = neutrino.CPictureViewerGui()
	
	if fileBrowser:getSelectedFile() ~= null then
		player:addToPlaylist(fileBrowser:getSelectedFile())
		player:exec(None, "")
	end

	if fileBrowser:getExitPressed() ~= true then
		pictureViewer()
	end
end

-- CMoviePlayerGui
function moviePlayer()
	fileBrowser = neutrino.CFileBrowser()
	fileFilter = neutrino.CFileFilter()
	fileFilter:addFilter("ts")

	fileBrowser.Multi_Select = false
	fileBrowser.Dirs_Selectable = false
	fileBrowser.Filter = fileFilter

	fileBrowser:exec(PATH)

	PATH = fileBrowser:getCurrentDir()

	player = neutrino.CMoviePlayerGui()
	
	if fileBrowser:getSelectedFile() ~= null then
		player:addToPlaylist(fileBrowser:getSelectedFile())
		player:exec(None, "")
	end

	if fileBrowser:getExitPressed() ~= true then
		moviePlayer()
	end
end

listWidget = neutrino.ClistBoxWidget("luaTest:ClistBoxWidget", neutrino.NEUTRINO_ICON_MOVIE)

function showMenu()
	print("showMenu")

	--listWidget:setSelected(selected)
	listWidget:setWidgetType(neutrino.WIDGET_TYPE_CLASSIC)
	listWidget:setMode(neutrino.MODE_MENU)
	listWidget:enableShrinkMenu()
	listWidget:enableWidgetChange()
	listWidget:enablePaintFootInfo()

	-- CMessageBox
	item1 = neutrino.CMenuForwarder("CMessageBox", true, "", null, "red action")
	item1:setItemIcon(neutrino.DATADIR .. "/neutrino/icons/plugin.png")
	item1:setHelpText("testing CMessageBox")

	-- CHelpBox
	item2 = neutrino.CMenuForwarder("CHelpBox")
	item2:setItemIcon(neutrino.DATADIR .. "/neutrino/icons/plugin.png")
	item2:setHelpText("testing CHelpBox")

	-- CHintBox
	item3 = neutrino.CMenuForwarder("CHintBox")
	item3:setItemIcon(neutrino.DATADIR .. "/neutrino/icons/plugin.png")
	item3:setHelpText("testing CHintBox")

	-- CInfoBox
	item4 = neutrino.CMenuForwarder("CInfoBox")
	item4:setItemIcon(neutrino.DATADIR .. "/neutrino/icons/plugin.png")
	item4:setHelpText("testing CInfoBox")

	-- CStringInput
	item5 = neutrino.CMenuForwarder("CStringInput")
	item5:setItemIcon(neutrino.DATADIR .. "/neutrino/icons/plugin.png")
	item5:setHelpText("testing CStringInput")

	-- CAudioPlayerGui
	item6 = neutrino.CMenuForwarder("CAudioPlayerGui")
	item6:setItemIcon(neutrino.DATADIR .. "/neutrino/icons/plugin.png")
	item6:setHelpText("testing CAudioPlayerGui")

	-- CPictureViewerGui
	item7 = neutrino.CMenuForwarder("CPictureViewerGui")
	item7:setItemIcon(neutrino.DATADIR .. "/neutrino/icons/plugin.png")
	item7:setHelpText("testing CPictureViewerGui")

	-- CMoviePlayerGui
	item8 = neutrino.CMenuForwarder("CMoviePlayerGui")
	item8:setItemIcon(neutrino.DATADIR .. "/neutrino/icons/plugin.png")
	item8:setHelpText("testing CMoviePlayerGui")

	listWidget:addItem(item1)
	listWidget:addItem(item2)
	listWidget:addItem(item3)
	listWidget:addItem(item4)
	listWidget:addItem(item5)
	listWidget:addItem(item6)
	listWidget:addItem(item7)
	listWidget:addItem(item8)

	listWidget:addKey(neutrino.RC_info)

	listWidget:exec(null, "")

	selected = listWidget:getSelected()
	key = listWidget:getKey()

	if key == neutrino.RC_info then
		messageBox()
	end

	if selected == 0 then
		messageBox()
	end
	if selected == 1 then
		helpBox()
	end
	if selected == 2 then
		hintBox()
	end
	if selected == 3 then
		infoBox()
	end
	if selected == 4 then
		stringInput()
	end
	if selected == 5 then
		audioPlayer()
	end
	if selected == 6 then
		pictureViewer()
	end
	if selected == 7 then
		moviePlayer()
	end

	if listWidget:getExitPressed() == false then
		listWidget:clearItems()
		showMenu()
	end
end

-- main
function main()
	showMenu()
end

main()





