/*
 *  Copyright (c) 2012 The orxCraft Project
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 */

/**
 * @file CEGUIListPopup.cpp
 * @date 2013-04-20
 * @author graag@o2.pl
 *
 */
#include "CEGUIListPopup.h"

#include <string>

#include "orxCraft.h"
#include "orx_config_util.h"
#include "DialogManager.h"
#include "ScrollListbox.h"
#include "ObjectEditor.h"

using std::string;
using CEGUI::Event;
using CEGUI::Window;
using CEGUI::FrameWindow;

CEGUIListPopup::CEGUIListPopup(const string& name, const string& title):
    ListPopup(name, title)
{
}

void CEGUIListPopup::Init ()
{
    orxASSERT(
	    CEGUI::WindowManager::getSingleton().isWindowPresent(
		m_windowName)
	    );
    Window* window = CEGUI::WindowManager::getSingleton().getWindow(
	    m_windowName);

    // Subscribe to close window event
    window->subscribeEvent (FrameWindow::EventCloseClicked,
	    Event::Subscriber (&CEGUIListPopup::OnCloseClicked, this));

    if(! m_title.empty())
	window->setText(m_title);
    else
	window->setText(m_name);

    m_contentList = FindListbox("ListPopup/SelectionList");
}

bool CEGUIListPopup::OnCloseClicked (const CEGUI::EventArgs &e)
{
    OnDestroy();

    // Notify that the event was handled
    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
