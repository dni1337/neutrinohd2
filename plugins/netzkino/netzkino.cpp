 /*
  $Id: netzkino.cpp 2016.02.13 mohousch Exp $

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

#include <netzkino.h>

extern "C" void plugin_exec(void);
extern "C" void plugin_init(void);
extern "C" void plugin_del(void);

#define NEUTRINO_ICON_NETZKINO			PLUGINDIR "/netzkino/netzkino.png"
#define NEUTRINO_ICON_NETZKINO_SMALL		PLUGINDIR "/netzkino/netzkino_small.png"
  
//
CNKMovies::CNKMovies(int mode, int id, std::string title)
{
	dprintf(DEBUG_NORMAL, "CNKMovies: mode:%d id:%d title:%s\n", mode, id, title.c_str());

	moviesMenu = NULL;
	item = NULL;

	catMode = mode;
	catID = id;
	caption = title;
}

CNKMovies::~CNKMovies()
{
	dprintf(DEBUG_NORMAL, "~CNKMovies:\n");

	m_vMovieInfo.clear();
	nkparser.Cleanup();
	nksearch.clear();
}

void CNKMovies::hide()
{
	CFrameBuffer::getInstance()->paintBackground();
	CFrameBuffer::getInstance()->blit();
}

void CNKMovies::loadNKTitles(int mode, std::string search, int id)
{
	dprintf(DEBUG_NORMAL, "CNKMovies::loadNKTitles: (mode:%d) search:%s (id:%d)\n", mode, search.c_str(), id);

	CHintBox loadBox(LOCALE_NETZKINO, g_Locale->getText(LOCALE_MOVIEBROWSER_SCAN_FOR_MOVIES));
	loadBox.paint();

	nkparser.Cleanup();

	//
	if (nkparser.ParseFeed((cNKFeedParser::nk_feed_mode_t)mode, search, id)) 
	{
		nkparser.DownloadThumbnails();
	} 
	else 
	{
		//FIXME show error
		MessageBox(LOCALE_MESSAGEBOX_ERROR, g_Locale->getText(LOCALE_NK_MOVIES_ERROR), mbrCancel, mbCancel, NEUTRINO_ICON_ERROR);
		
		return;
	}
	
	m_vMovieInfo.clear();
	nk_video_list_t &ylist = nkparser.GetVideoList();
	
	for (unsigned int count = 0; count < ylist.size(); count++) 
	{
		MI_MOVIE_INFO movieInfo;
		m_movieInfo.clearMovieInfo(&movieInfo); // refresh structure
		
		movieInfo.epgTitle = ylist[count].title;
		movieInfo.epgInfo2 = ylist[count].description;
		movieInfo.tfile = ylist[count].tfile;
		movieInfo.ytdate = ylist[count].published;
		movieInfo.ytid = ylist[count].id;
		movieInfo.file.Name = ylist[count].url;
		
		m_vMovieInfo.push_back(movieInfo);
	}

	loadBox.hide();
}

#define NK_HEAD_BUTTONS_COUNT	2
const struct button_label NKHeadButtons[NK_HEAD_BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_HELP, NONEXISTANT_LOCALE, NULL },
	{ NEUTRINO_ICON_BUTTON_SETUP, NONEXISTANT_LOCALE, NULL}
};

void CNKMovies::showMenu()
{
	dprintf(DEBUG_NORMAL, "CNKMovies::showMenu: mode:%d id:%d title:%s\n", catMode, catID, caption.c_str());
	
	if(m_vMovieInfo.empty())
	{
		MessageBox(LOCALE_MESSAGEBOX_ERROR, g_Locale->getText(LOCALE_NK_ERROR), mbrCancel, mbCancel, NEUTRINO_ICON_ERROR);
		
		return;
	}

	//
	std::string title;
	
	title = "NetzKino";
	title += ": ";
	title += caption;

	moviesMenu = new ClistBoxWidget(title.c_str(), NEUTRINO_ICON_NETZKINO_SMALL, w_max ( (CFrameBuffer::getInstance()->getScreenWidth() / 20 * 17), (CFrameBuffer::getInstance()->getScreenWidth() / 20 )), h_max ( (CFrameBuffer::getInstance()->getScreenHeight() / 20 * 16), (CFrameBuffer::getInstance()->getScreenHeight() / 20)));

	for (unsigned int i = 0; i < m_vMovieInfo.size(); i++)
	{
		item = new ClistBoxItem(m_vMovieInfo[i].epgTitle.c_str(), true, NULL, this, "play", RC_nokey, NULL, file_exists(m_vMovieInfo[i].tfile.c_str())? m_vMovieInfo[i].tfile.c_str() : DATADIR "/neutrino/icons/nopreview.jpg");

		item->setHelpText(m_vMovieInfo[i].epgInfo2.c_str());

		//
		moviesMenu->addItem(item);
	}

	moviesMenu->setMode(MODE_LISTBOX);
	moviesMenu->setWidgetType(WIDGET_TYPE_FRAME);
	moviesMenu->setItemsPerPage(6, 2);
	//moviesMenu->setItemBoxColor(COL_YELLOW);

	moviesMenu->setHeaderButtons(NKHeadButtons, NK_HEAD_BUTTONS_COUNT);

	moviesMenu->addKey(RC_info, this, CRCInput::getSpecialKeyName(RC_info));
	moviesMenu->addKey(RC_setup, this, CRCInput::getSpecialKeyName(RC_setup));
	moviesMenu->addKey(RC_record, this, CRCInput::getSpecialKeyName(RC_record));

	moviesMenu->exec(NULL, "");
	//moviesMenu->hide();
	delete moviesMenu;
	moviesMenu = NULL;
}

void CNKMovies::playMovie(void)
{
	if (&m_vMovieInfo[moviesMenu->getSelected()].file != NULL) 
	{
		tmpMoviePlayerGui.addToPlaylist(m_vMovieInfo[moviesMenu->getSelected()]);
		tmpMoviePlayerGui.exec(NULL, "");
	}
}

void CNKMovies::showMovieInfo(void)
{
	m_movieInfo.showMovieInfo(m_vMovieInfo[moviesMenu->getSelected()]);
}

void CNKMovies::recordMovie(void)
{
	std::string infoString;

	MI_MOVIE_INFO g_movieInfo;
	m_movieInfo.clearMovieInfo(&g_movieInfo); // refresh structure
		
	g_movieInfo.epgTitle = m_vMovieInfo[moviesMenu->getSelected()].epgTitle;
	g_movieInfo.epgInfo2 = m_vMovieInfo[moviesMenu->getSelected()].epgInfo2;
	g_movieInfo.tfile = m_vMovieInfo[moviesMenu->getSelected()].tfile;
	g_movieInfo.ytdate = m_vMovieInfo[moviesMenu->getSelected()].ytdate;
	g_movieInfo.ytid = m_vMovieInfo[moviesMenu->getSelected()].ytid;
	g_movieInfo.file.Name = m_vMovieInfo[moviesMenu->getSelected()].file.Name;

	m_movieInfo.encodeMovieInfoXml(&infoString, &g_movieInfo);

	::start_file_recording(m_vMovieInfo[moviesMenu->getSelected()].epgTitle.c_str(), infoString.c_str(), m_vMovieInfo[moviesMenu->getSelected()].file.Name.c_str());
}

int CNKMovies::showCategoriesMenu()
{
	dprintf(DEBUG_NORMAL, "CNKMovies::showCategoriesMenu:\n");

	int res = -1;

	// load Categories
	CHintBox loadBox(LOCALE_NETZKINO, g_Locale->getText(LOCALE_NK_SCAN_FOR_CATEGORIES));
	
	loadBox.paint();

	cats = nkparser.GetCategoryList();

	loadBox.hide();

	if(cats.empty())
	{
		//FIXME show error
		MessageBox(LOCALE_MESSAGEBOX_ERROR, g_Locale->getText(LOCALE_NK_ERROR), mbrCancel, mbCancel, NEUTRINO_ICON_ERROR);
		
		return false;
	}

	// menu
	ClistBoxWidget mainMenu(LOCALE_NETZKINO, NEUTRINO_ICON_NETZKINO_SMALL);

	mainMenu.setMode(MODE_MENU);
	mainMenu.enableShrinkMenu();
	mainMenu.setWidgetType(WIDGET_TYPE_CLASSIC);

	// categories
	for (unsigned i = 0; i < cats.size(); i++)
	{
		mainMenu.addItem(new CMenuForwarder(cats[i].title.c_str(), true, NULL, new CNKMovies(cNKFeedParser::CATEGORY, cats[i].id, cats[i].title), "", RC_nokey, NULL, NEUTRINO_ICON_NETZKINO));
	}

	// search
	mainMenu.addItem(new CMenuSeparator(LINE));

	mainMenu.addItem(new CMenuForwarder(LOCALE_YT_SEARCH, true, nksearch.c_str(), this, "search", RC_nokey, NULL, NEUTRINO_ICON_NETZKINO));

	mainMenu.exec(NULL, "");
	res = mainMenu.getSelected();

	return res;
}

int CNKMovies::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CNKMovies::exec: actionKey:%s\n", actionKey.c_str());

	if(parent) 
		parent->hide();

	if(actionKey == "play")
	{
		playMovie();

		return menu_return::RETURN_REPAINT;
	}
	else if(actionKey == "RC_info")
	{
		showMovieInfo();

		return menu_return::RETURN_REPAINT;
	}
	else if(actionKey == "RC_setup")
	{
		int res = showCategoriesMenu();
		
		if(res >= 0 && res <= 24)
			return menu_return::RETURN_EXIT_ALL;
		else
			return menu_return::RETURN_REPAINT;
	}
	else if(actionKey == "RC_record")
	{
		recordMovie();
		return menu_return::RETURN_REPAINT;
	}
	else if(actionKey == "search")
	{
		nksearch.clear();

		CStringInputSMS stringInput(LOCALE_YT_SEARCH, nksearch.c_str());
		int ret = stringInput.exec(NULL, "");

		printf("ret:%d nksearch:%s\n", ret, nksearch.c_str());

		if(ret) //FIXME:
		{
			loadNKTitles(cNKFeedParser::SEARCH, nksearch, 0);
			showMenu();

			return menu_return::RETURN_EXIT_ALL;
		}
		else
			return menu_return::RETURN_REPAINT;
	}

	loadNKTitles(catMode, caption, catID);
	showMenu();
	
	return menu_return::RETURN_EXIT;
}

// plugin API
void plugin_init(void)
{
}

void plugin_del(void)
{
}

void plugin_exec(void)
{
	CNKMovies * NKHandler = new CNKMovies(cNKFeedParser::CATEGORY, 8, "Highlights");
	
	NKHandler->exec(NULL, "");
	
	delete NKHandler;
	NKHandler = NULL;			
}


