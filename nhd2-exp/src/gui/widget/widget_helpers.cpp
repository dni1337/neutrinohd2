/*
 * $Id: widget_helpers.cpp 27.02.2019 mohousch Exp $
 *
 * (C) 2003 by thegoodguy <thegoodguy@berlios.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <global.h>

#include <driver/color.h>
#include <system/settings.h>

#include <gui/widget/widget_helpers.h>


CButtons::CButtons()
{
}

void CButtons::paintFootButtons(const int x, const int y, const int dx, const int dy, const unsigned int count, const struct button_label * const content)
{
	int iw, ih;

	int buttonWidth = 0;
	
	if(count)
	{
		buttonWidth = (dx - BORDER_LEFT - BORDER_RIGHT)/count;

		for (unsigned int i = 0; i < count; i++)
		{
			if(content[i].button != NULL)
			{
				//const char * l_option = NULL;
				std::string l_option("");

				l_option.clear();

				CFrameBuffer::getInstance()->getIconSize(content[i].button, &iw, &ih);
				int f_h = g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->getHeight();

				if(content[i].localename != NULL)
					l_option = content[i].localename;
				else
					l_option = g_Locale->getText(content[i].locale);
		
				CFrameBuffer::getInstance()->paintIcon(content[i].button, x + BORDER_LEFT + i*buttonWidth, y + (dy - ih)/2);

				g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x + BORDER_LEFT + iw + ICON_OFFSET + i*buttonWidth, y + f_h + (dy - f_h)/2, buttonWidth - iw - ICON_OFFSET, l_option, COL_MENUFOOT, 0, true); // UTF-8
			}
		}
	}
}

// head buttons (right)
void CButtons::paintHeadButtons(const int x, const int y, const int dx, const int dy, const unsigned int count, const struct button_label * const content)
{
	int iw[count], ih[count];
	int startx = x + dx - BORDER_RIGHT;
	
	for (unsigned int i = 0; i < count; i++)
	{
		if(content[i].button != NULL)
		{
			CFrameBuffer::getInstance()->getIconSize(content[i].button, &iw[i], &ih[i]);
		
			startx -= (iw[i] + ICON_TO_ICON_OFFSET);

			CFrameBuffer::getInstance()->paintIcon(content[i].button, startx, y + (dy - ih[i])/2);
		}
	}
}

// scrollBar
void CScrollBar::paint(const int x, const int y, const int dy, const int NrOfPages, const int CurrentPage)
{
	// scrollBar
	CBox cFrameScrollBar;
	CWindow cScrollBarWindow;

	cFrameScrollBar.iX = x;
	cFrameScrollBar.iY = y;
	cFrameScrollBar.iWidth = SCROLLBAR_WIDTH;
	cFrameScrollBar.iHeight = dy;


	cScrollBarWindow.setPosition(&cFrameScrollBar);
	cScrollBarWindow.setColor(COL_SCROLLBAR);
	cScrollBarWindow.setCorner(NO_RADIUS, CORNER_ALL);
	cScrollBarWindow.paint();
		
	// scrollBar slider
	CBox cFrameSlider;
	CWindow cSliderWindow;	

	cFrameSlider.iX = cFrameScrollBar.iX + 2;
	cFrameSlider.iY = cFrameScrollBar.iY + CurrentPage*(cFrameScrollBar.iHeight/NrOfPages);
	cFrameSlider.iWidth = cFrameScrollBar.iWidth - 4;
	cFrameSlider.iHeight = cFrameScrollBar.iHeight/NrOfPages;

	cSliderWindow.setPosition(&cFrameSlider);
	cSliderWindow.setColor(COL_SCROLLBAR_SLIDER);
	cSliderWindow.setCorner(NO_RADIUS, CORNER_ALL);
	cSliderWindow.paint();
}

void CScrollBar::paint(CBox* position, const int NrOfPages, const int CurrentPage)
{
	// scrollBar
	CBox cFrameScrollBar;
	CWindow cScrollBarWindow;

	cFrameScrollBar = *position;

	cScrollBarWindow.setPosition(&cFrameScrollBar);
	cScrollBarWindow.setColor(COL_SCROLLBAR);
	cScrollBarWindow.setCorner(NO_RADIUS, CORNER_ALL);
	cScrollBarWindow.paint();
		
	// scrollBar slider
	CBox cFrameSlider;
	CWindow cSliderWindow;	

	cFrameSlider.iX = cFrameScrollBar.iX + 2;
	cFrameSlider.iY = cFrameScrollBar.iY + CurrentPage*(cFrameScrollBar.iHeight/NrOfPages);
	cFrameSlider.iWidth = cFrameScrollBar.iWidth - 4;
	cFrameSlider.iHeight = cFrameScrollBar.iHeight/NrOfPages;

	cSliderWindow.setPosition(&cFrameSlider);
	cSliderWindow.setColor(COL_SCROLLBAR_SLIDER);
	cSliderWindow.setCorner(NO_RADIUS, CORNER_ALL);
	cSliderWindow.paint();
}

// detailsLine
void CItems2DetailsLine::paint(int x, int y, int width, int height, int info_height/*, int theight*/, int iheight, int iy)
{
	int xpos  = x - ConnectLineBox_Width;
	int ypos1 = iy;
	int ypos2 = y + height;
	int ypos1a = ypos1 + (iheight/2) - 2;
	int ypos2a = ypos2 + (info_height/2) - 2;
	
	fb_pixel_t col1 = COL_MENUCONTENT_PLUS_6;
	fb_pixel_t col2 = COL_MENUFOOT_INFO_PLUS_0;

	// clear
	CFrameBuffer::getInstance()->paintBackgroundBoxRel(xpos, y, ConnectLineBox_Width, height + info_height);

	// paint Line if detail info (and not valid list pos)
	//if (pos >= 0) 
	{ 
		int fh = iheight > 10 ? iheight - 10 : 5;
		
		// vertical line connected to item	
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 4, ypos1 + 5, 4, fh, col1);
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 4, ypos1 + 5, 1, fh, col2);
		
		// vertical line connected to infobox	
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 4, ypos2 + 7, 4, info_height - 14, col1);
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 4, ypos2 + 7, 1, info_height - 14, col2);			

		// vertical line
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 16, ypos1a, 4, ypos2a - ypos1a, col1);
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 16, ypos1a, 1, ypos2a - ypos1a + 4, col2);		

		// Hline (item)
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 15, ypos1a, 12, 4, col1);
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 16, ypos1a, 12, 1, col2);
		
		// Hline (infobox)
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 15, ypos2a, 12, 4, col1);
		CFrameBuffer::getInstance()->paintBoxRel(xpos + ConnectLineBox_Width - 12, ypos2a, 8, 1, col2);

		// untere info box background
		CFrameBuffer::getInstance()->paintBoxRel(x, ypos2, width, info_height, col1);
		CFrameBuffer::getInstance()->paintBoxRel(x + 2, ypos2 + 2, width - 4, info_height - 4, COL_MENUFOOT_INFO_PLUS_0, NO_RADIUS, CORNER_NONE, g_settings.Foot_Info_gradient);	
	}
}


void CItems2DetailsLine::clear(int x, int y, int width, int height, int info_height)
{ 
	// lines
	CFrameBuffer::getInstance()->paintBackgroundBoxRel(x - ConnectLineBox_Width, y, ConnectLineBox_Width, height + info_height);

	// lines around info box
	CFrameBuffer::getInstance()->paintBackgroundBoxRel(x, y + height, width, info_height);
}

// headers
CHeaders::CHeaders()
{
	// head
	bgcolor = COL_MENUHEAD_PLUS_0;
	radius = RADIUS_MID;
	corner = CORNER_TOP;
	gradient = g_settings.Head_gradient;

	paintDate = false;
	logo = false;
	hbutton_count	= 0;
	hbutton_labels	= NULL;

	// foot
	fbgcolor = COL_MENUFOOT_PLUS_0;
	fradius = RADIUS_MID;
	fcorner = CORNER_BOTTOM;
	fgradient = g_settings.Foot_gradient;
}

void CHeaders::setHeaderButtons(const struct button_label* _hbutton_labels, const int _hbutton_count)
{
	hbutton_count = _hbutton_count;
	hbutton_labels = _hbutton_labels;
}

void CHeaders::paintHead(const int x, const int y, const int dx, const int dy, const char * const title, const char * const icon)
{
	// box
	CFrameBuffer::getInstance()->paintBoxRel(x, y, dx, dy, bgcolor, radius, corner, gradient);

	// left icon
	int i_w = 0;
	int i_h = 0;
	if(icon != NULL)
	{
		CFrameBuffer::getInstance()->getIconSize(icon, &i_w, &i_h);

		// limit icon dimensions
		if(i_h > dy)
			i_h = dy - 2;

		if(logo)
		{
			i_w = i_h*1.67;

			CFrameBuffer::getInstance()->paintIcon(icon, x + BORDER_LEFT, y + (dy - i_h)/2, 0, true, i_w, i_h);
		}
		else
			CFrameBuffer::getInstance()->paintIcon(icon, x + BORDER_LEFT, y + (dy - i_h)/2);
	}

	// right buttons
	int iw[hbutton_count], ih[hbutton_count];
	int startx = x + dx - BORDER_RIGHT;
	int buttonWidth = 0;

	if(hbutton_count)
	{
		for (int i = 0; i < hbutton_count; i++)
		{
			CFrameBuffer::getInstance()->getIconSize(hbutton_labels[i].button, &iw[i], &ih[i]);
		
			startx -= (iw[i] + ICON_TO_ICON_OFFSET);
			buttonWidth += iw[i];

			CFrameBuffer::getInstance()->paintIcon(hbutton_labels[i].button, startx, y + (dy - ih[i])/2);
		}
	}

	// paint time/date
	int timestr_len = 0;
	if(paintDate)
	{
		std::string timestr = getNowTimeStr("%d.%m.%Y %H:%M");;
		
		timestr_len = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getRenderWidth(timestr.c_str(), true); // UTF-8
	
		g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->RenderString(startx - timestr_len, y + (dy - g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight(), timestr_len + 1, timestr.c_str(), COL_MENUHEAD, 0, true); 
	}

	// title
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(x + BORDER_LEFT + i_w + ICON_OFFSET, y + (dy - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), dx - BORDER_LEFT - BORDER_RIGHT - i_w - 2*ICON_OFFSET - buttonWidth - (hbutton_count - 1)*ICON_TO_ICON_OFFSET - timestr_len, title, COL_MENUHEAD);
	
}

void CHeaders::paintHead(CBox position, const char * const title, const char * const icon)
{
	// box
	CFrameBuffer::getInstance()->paintBoxRel(position.iX, position.iY, position.iWidth, position.iHeight, bgcolor, radius, corner, gradient);

	// left icon
	int i_w = 0;
	int i_h = 0;
	if(icon != NULL)
	{
		CFrameBuffer::getInstance()->getIconSize(icon, &i_w, &i_h);

		// limit icon dimensions
		if(i_h > position.iHeight)
			i_h = position.iHeight - 2;

		if(logo)
		{
			i_w = i_h*1.67;

			CFrameBuffer::getInstance()->paintIcon(icon, position.iX + BORDER_LEFT, position.iY + (position.iHeight - i_h)/2, 0, true, i_w, i_h);
		}
		else
			CFrameBuffer::getInstance()->paintIcon(icon, position.iX + BORDER_LEFT, position.iY + (position.iHeight - i_h)/2);
	}

	// right buttons
	int iw[hbutton_count], ih[hbutton_count];
	int startx = position.iX + position.iWidth - BORDER_RIGHT;
	int buttonWidth = 0;

	if(hbutton_count)
	{
		for (int i = 0; i < hbutton_count; i++)
		{
			CFrameBuffer::getInstance()->getIconSize(hbutton_labels[i].button, &iw[i], &ih[i]);
		
			startx -= (iw[i] + ICON_TO_ICON_OFFSET);
			buttonWidth += iw[i];

			CFrameBuffer::getInstance()->paintIcon(hbutton_labels[i].button, startx, position.iY + (position.iHeight - ih[i])/2);
		}
	}

	// paint time/date
	int timestr_len = 0;
	if(paintDate)
	{
		std::string timestr = getNowTimeStr("%d.%m.%Y %H:%M");;
		
		timestr_len = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getRenderWidth(timestr.c_str(), true); // UTF-8
	
		g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->RenderString(startx - timestr_len, position.iY + (position.iHeight - g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight(), timestr_len + 1, timestr.c_str(), COL_MENUHEAD, 0, true); 
	}

	// title
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(position.iX + BORDER_LEFT + i_w + ICON_OFFSET, position.iY + (position.iHeight - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), position.iWidth - BORDER_LEFT - BORDER_RIGHT - i_w - 2*ICON_OFFSET - buttonWidth - (hbutton_count - 1)*ICON_TO_ICON_OFFSET - timestr_len, title, COL_MENUHEAD);
}

// foot
void CHeaders::paintFoot(int x, int y, int dx, int dy, const unsigned int count, const struct button_label * const content)
{
	// box
	CFrameBuffer::getInstance()->paintBoxRel(x, y, dx, dy, COL_MENUFOOT_PLUS_0, RADIUS_MID, CORNER_BOTTOM, g_settings.Foot_gradient);


	// buttons
	int buttonWidth = 0;

	if(count)
	{
		buttonWidth = (dx - BORDER_LEFT - BORDER_RIGHT)/count;
	
		for (unsigned int i = 0; i < count; i++)
		{
			if(content[i].button != NULL)
			{
				const char * l_option = NULL;
				int iw = 0;
				int ih = 0;

				CFrameBuffer::getInstance()->getIconSize(content[i].button, &iw, &ih);
				int f_h = g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->getHeight();

				if(content[i].localename != NULL)
					l_option = content[i].localename;
				else
					l_option = g_Locale->getText(content[i].locale);
		
				CFrameBuffer::getInstance()->paintIcon(content[i].button, x + BORDER_LEFT + i*buttonWidth, y + (dy - ih)/2);

				g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x + BORDER_LEFT + iw + ICON_OFFSET + i*buttonWidth, y + f_h + (dy - f_h)/2, buttonWidth - iw - ICON_OFFSET, l_option, COL_MENUFOOT, 0, true); // UTF-8
			}
		}
	}
}

void CHeaders::paintFoot(CBox position, const unsigned int count, const struct button_label * const content)
{
	// box
	CFrameBuffer::getInstance()->paintBoxRel(position.iX, position.iY, position.iWidth, position.iHeight, fbgcolor, fradius, fcorner, fgradient);

	// buttons
	int buttonWidth = 0;
	if(count)
	{
		buttonWidth = (position.iWidth - BORDER_LEFT - BORDER_RIGHT)/count;
	
		for (unsigned int i = 0; i < count; i++)
		{
			if(content[i].button != NULL)
			{
				const char * l_option = NULL;
				int iw = 0;
				int ih = 0;

				CFrameBuffer::getInstance()->getIconSize(content[i].button, &iw, &ih);
				int f_h = g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->getHeight();

				if(content[i].localename != NULL)
					l_option = content[i].localename;
				else
					l_option = g_Locale->getText(content[i].locale);
		
				// paint button
				CFrameBuffer::getInstance()->paintIcon(content[i].button, position.iX + BORDER_LEFT+ i * buttonWidth, position.iY + (position.iHeight - ih)/2);

				// paint button caption
				g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(position.iX + BORDER_LEFT + iw + ICON_OFFSET + i*buttonWidth, position.iY + f_h + (position.iHeight - f_h)/2, buttonWidth - iw - ICON_OFFSET, l_option, COL_MENUFOOT, 0, true); // UTF-8
			}
		}
	}
}

// progressbar
#define ITEMW 1


CProgressBar::CProgressBar(int w, int h, int r, int g, int b, bool inv)
{
	frameBuffer = CFrameBuffer::getInstance();
	
	double div;
	width = w;
	height = h;
	inverse = inv;
	
	div = (double) 100 / (double) width;

	red = (double) r / (double) div / (double) ITEMW;
	green = (double) g / (double) div / (double) ITEMW;
	yellow = (double) b / (double) div / (double) ITEMW;
	
	percent = 255;
}

void CProgressBar::paint(unsigned int x, unsigned int y, unsigned char pcr)
{
	int i, siglen;
	unsigned int posx;
	unsigned int posy;
	unsigned int xpos;
	unsigned int ypos;

	double div;
	uint32_t rgb;
	
	fb_pixel_t color = COL_MENUCONTENT_PLUS_7;
	int b = 0;
	
	i = 0;
	xpos = x;
	ypos = y;

	frameBuffer->paintBoxRel(x, y, width, height, COL_MENUCONTENT_PLUS_2);	//fill passive
	
	if (pcr != percent) 
	{
		if(percent == 255) 
			percent = 0;

		div = (double) 100 / (double) width;
		siglen = (double) pcr / (double) div;
		posx = xpos;
		posy = ypos;
		int maxi = siglen/ITEMW;
		int total = width/ITEMW;
		int step = 255/total;

		if (pcr > percent) 
		{
			if(g_settings.progressbar_color == 0)
			{
				//red
				for (i = 0; (i < red) && (i < maxi); i++) 
				{
					step = 255/red;

					if(inverse) 
						rgb = COL_GREEN + ((unsigned char)(step*i) << 16); // adding red
					else
						rgb = COL_RED + ((unsigned char)(step*i) <<  8); // adding green
				
					color = rgb;
				
					frameBuffer->paintBoxRel(posx + i*ITEMW, posy, ITEMW, height, color);
				}
	
				//yellow
				for (; (i < yellow) && (i < maxi); i++) 
				{
					step = 255/yellow/2;

					if(inverse) 
						rgb = COL_YELLOW - (((unsigned char)step*(b++)) <<  8); // removing green
					else
						rgb = COL_YELLOW - ((unsigned char)(step*(b++)) << 16); // removing red
	
					color = rgb;		    
				
					frameBuffer->paintBoxRel(posx + i*ITEMW, posy, ITEMW, height, color);
				}

				//green
				for (; (i < green) && (i < maxi); i++) 
				{
					step = 255/green;

					if(inverse) 
						rgb = COL_YELLOW - ((unsigned char) (step*(b++)) <<  8); // removing green
					else
						rgb = COL_YELLOW - ((unsigned char) (step*(b++)) << 16); // removing red
				
					color = rgb;
				
					frameBuffer->paintBoxRel (posx + i*ITEMW, posy, ITEMW, height, color);
				}
			}
			else
			{
				for(; (i < maxi); i++) 
				{
					frameBuffer->paintBoxRel(posx + i*ITEMW, posy, ITEMW, height, color);
				}
			}
		}
		
		percent = pcr;
	}
}

void CProgressBar::reset()
{
  	percent = 255;
}


