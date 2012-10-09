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
 * @file ListPopup.cpp
 * @date 2012-09-05
 * @author graag@o2.pl
 *
 */
#include "ListPopup.h"

#include <string>

#include "orxCraft.h"
#include "CEDialogManager.h"

using std::string;
using CEGUI::Event;
using CEGUI::Window;
using CEGUI::FrameWindow;

ListPopup::ListPopup(const string& name, const string& options):
    ScrollFrameWindow(name, options),
    m_contentList(NULL)
{}

void ListPopup::Init ()
{
    orxASSERT(
	    CEGUI::WindowManager::getSingleton().isWindowPresent(
		m_windowName)
	    );
    Window* window = CEGUI::WindowManager::getSingleton().getWindow(
	    m_windowName);

    // Subscribe to close window event
    window->subscribeEvent (FrameWindow::EventCloseClicked,
	    Event::Subscriber (&ListPopup::OnCloseClicked, this));
}

void ListPopup::OnMouseClick (const string& widgetName)
{
    orxASSERT (false);
}

void ListPopup::OnTextAccepted (const string& widgetName)
{
    orxASSERT (false);
}

void ListPopup::OnDestroy ()
{
    CEDialogManager::GetInstance()->DestroyDialog(m_id);
    /*
     * Beyond this point the dialog was destroyed (delete was issued).
     * Make sure in is not accessed anymore.
     */
}

bool ListPopup::OnCloseClicked (const CEGUI::EventArgs &e)
{
    OnDestroy();

    // Notify that the event was handled
    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
