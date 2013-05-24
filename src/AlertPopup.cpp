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
 * @file AlertPopup.cpp
 * @date 2013-04-26
 * @author graag@o2.pl
 *
 */
#include "AlertPopup.h"

#include <string>

#include "orxCraft.h"
#include "orx_config_util.h"
#include "DialogManager.h"
#include "ScrollListbox.h"

using std::string;

AlertPopup::AlertPopup(const string& name, const string& title):
    ScrollFrameWindow(name, title)
{
    // Popups should be modal
    m_isModal = true;
}

void AlertPopup::OnAction(const string& widgetName, const string& action)
{
    if (widgetName == "AlertPopup/DoneButton")
    {
	SignalFinish(m_name, m_id);
	OnClose();
    }
}

void AlertPopup::OnInput(const string& widgetName)
{
    orxASSERT (false);
}

void AlertPopup::HandlePopup(const string& popupName,
	orxU32 popupTitle)
{
    orxASSERT (false);
}

void AlertPopup::HandleClose(const string& popupName,
	orxU32 popupTitle)
{
    orxASSERT (false);
}

void AlertPopup::OnClose()
{
    SignalClose(m_name, m_id);
    OrxCraft::GetInstance().GetDialogManager()->CloseDialog(m_id);
    /*
     * Beyond this point the dialog was destroyed (delete was issued).
     * Make sure in is not accessed anymore.
     */
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
