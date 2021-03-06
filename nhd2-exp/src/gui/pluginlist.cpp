/*
	Neutrino-GUI  -   DBoxII-Project

	Copyright (C) 2001 Steffen Hehn 'McClean'
	Homepage: http://dbox.cyberphoria.org/

	Kommentar:

	Diese GUI wurde von Grund auf neu programmiert und sollte nun vom
	Aufbau und auch den Ausbaumoeglichkeiten gut aussehen. Neutrino basiert
	auf der Client-Server Idee, diese GUI ist also von der direkten DBox-
	Steuerung getrennt. Diese wird dann von Daemons uebernommen.


	License: GPL

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <global.h>
#include <neutrino.h>

#include <plugin.h>

#include <gui/pluginlist.h>
#include <gui/widget/infobox.h>
#include <gui/widget/icons.h>

#include <sstream>
#include <fstream>
#include <iostream>

#include <dirent.h>
#include <dlfcn.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <plugins.h>
#include <driver/encoding.h>

#include <system/debug.h>


extern CPlugins * g_PluginList;    /* neutrino.cpp */

CPluginList::CPluginList()
{
	frameBuffer = CFrameBuffer::getInstance();

	selected = 0;

	plist = NULL;
	item = NULL;
}

CPluginList::~CPluginList()
{
}

void CPluginList::hide()
{
	frameBuffer->paintBackground();
	frameBuffer->blit();	
}

#define NUM_LIST_BUTTONS 3
struct button_label CPluginListButtons[NUM_LIST_BUTTONS] =
{
	{ NEUTRINO_ICON_BUTTON_RED, LOCALE_PLUGINLIST_REMOVE_PLUGIN },
	{ NEUTRINO_ICON_BUTTON_GREEN, LOCALE_PLUGINLIST_START_PLUGIN },
	{ NEUTRINO_ICON_BUTTON_YELLOW, LOCALE_SERVICEMENU_GETPLUGINS }
};

struct button_label CPluginListHeadButtons = {NEUTRINO_ICON_BUTTON_HELP_SMALL, NONEXISTANT_LOCALE, NULL};

void CPluginList::showMenu()
{
	dprintf(DEBUG_NORMAL, "CPluginList::showMenu\n");


	// itemBox
	plist = new ClistBoxWidget(LOCALE_USERMENU_ITEM_PLUGINS, NEUTRINO_ICON_SHELL, MENU_WIDTH, MENU_HEIGHT - 100);

	//
	for(unsigned int count = 0; count < (unsigned int)g_PluginList->getNumberOfPlugins(); count++)
	{
		std::string IconName = "";

		IconName = PLUGINDIR;
		IconName += "/";
		IconName += g_PluginList->getFileName(count);
		IconName += "/";
		IconName += g_PluginList->getIcon(count);

		bool enabled = g_PluginList->getType(count) != CPlugins::P_TYPE_DISABLED;
			
		item = new ClistBoxItem(g_PluginList->getName(count), enabled, g_PluginList->getDescription(count).c_str(), CPluginsExec::getInstance(), to_string(count).c_str(), RC_nokey, NULL, file_exists(IconName.c_str())? IconName.c_str() : NEUTRINO_ICON_MENUITEM_PLUGIN);

		item->setInfo1(g_PluginList->getDescription(count).c_str());

		item->set2lines(); 

		plist->addItem(item);
	}

	plist->setMode(MODE_LISTBOX);
	plist->setWidgetType(WIDGET_TYPE_CLASSIC);
	plist->enablePaintDate();

	plist->setSelected(selected);

	// head
	plist->setHeaderButtons(&CPluginListHeadButtons, 1);

	// footer
	plist->setFooterButtons(CPluginListButtons, NUM_LIST_BUTTONS);

	//
	plist->addKey(RC_red, this, CRCInput::getSpecialKeyName(RC_red));
	plist->addKey(RC_green, this, CRCInput::getSpecialKeyName(RC_green));
	plist->addKey(RC_yellow, this, CRCInput::getSpecialKeyName(RC_yellow));
	plist->addKey(RC_info, this, CRCInput::getSpecialKeyName(RC_info));
	plist->addKey(RC_ok, this, CRCInput::getSpecialKeyName(RC_ok));

	plist->exec(NULL, "");
	//plist->hide();
	delete plist;
	plist = NULL;
}

int CPluginList::exec(CMenuTarget * parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CPluginList::exec\n");

	if (parent)
		parent->hide();

	if(actionKey == "RC_red")
	{
		selected = plist->getSelected();

		// remove selected plugin
		g_PluginList->removePlugin(selected);

		// relaod plugins
		g_PluginList->loadPlugins();

		if(selected > (int)g_PluginList->getNumberOfPlugins() - 1)
			selected = (int)g_PluginList->getNumberOfPlugins() - 1;

		showMenu();
		return menu_return::RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_green")
	{
		selected = plist->getSelected();
		g_PluginList->startPlugin(plist->getSelected());
	}
	else if(actionKey == "RC_yellow")
	{
		g_PluginList->loadPlugins();
		showMenu();
		return menu_return::RETURN_EXIT_ALL;
	}
	else if(actionKey == "RC_info")
	{
		selected = plist->getSelected();
		std::string buffer;

		buffer = "Name: ";
		buffer += g_PluginList->getName(selected);
		buffer += "\n";

		if(!g_PluginList->getDescription(selected).empty())
		{
			buffer += "Description: ";
			buffer += g_PluginList->getDescription(selected);
			buffer += "\n";
		}

		if(!g_PluginList->getVersion(selected).empty())
		{
			buffer += "Version: ";
			buffer += g_PluginList->getVersion(selected);
			buffer += "\n";
		}
		
		InfoBox(buffer.c_str(), g_Locale->getText(LOCALE_USERMENU_ITEM_PLUGINS), NEUTRINO_ICON_SHELL);
		return menu_return::RETURN_REPAINT;
	}
	else if(actionKey == "RC_ok")
	{
		selected = plist->getSelected();

		if(pluginSelected() == close)
			return menu_return::RETURN_EXIT_ALL;
		else
			return menu_return::RETURN_REPAINT;
	}

	showMenu();

	return menu_return::RETURN_REPAINT;
}

CPluginList::result_ CPluginList::pluginSelected()
{
	g_PluginList->startPlugin(plist->getSelected());
	
	return resume;
}

CPluginChooser::CPluginChooser(char* pluginname)
	: CPluginList(), selected_plugin(pluginname)
{
}

CPluginList::result_ CPluginChooser::pluginSelected()
{
	strcpy(selected_plugin, g_PluginList->getFileName(plist->getSelected()));

	return CPluginList::close;
}

//
CPluginsExec* CPluginsExec::getInstance()
{
	static CPluginsExec* pluginsExec = NULL;

	if (!pluginsExec)
		pluginsExec = new CPluginsExec();

	return pluginsExec;
}

int CPluginsExec::exec(CMenuTarget* parent, const std::string & actionKey)
{
	if (actionKey.empty())
		return menu_return::RETURN_NONE;

	dprintf(DEBUG_NORMAL, "CPluginsExec exec: %s\n", actionKey.c_str());

	int sel = atoi(actionKey.c_str());

	if (parent != NULL)
		parent->hide();

	if (sel >= 0)
		g_PluginList->startPlugin(sel);

	if (g_PluginList->getIntegration(sel) == CPlugins::I_TYPE_DISABLED)
		return menu_return::RETURN_EXIT;

	return menu_return::RETURN_REPAINT;
}


