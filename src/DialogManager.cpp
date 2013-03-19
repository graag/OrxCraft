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
 * @file DialogManager.cpp
 * @date 2012-07-01
 * @author fritz@fritzmahnke.com
 *
 * Implementation of the Dialog Manager.
 *
 */

#include "DialogManager.h"

#include <string>
#include <vector>

#include "ScrollFrameWindow.h"

using std::string;

DialogManager* DialogManager::m_instance = NULL;

ScrollFrameWindow* DialogManager::GetDialog(const string& dialogName,
        const string& dialogTitle)
{
    ScrollFrameWindow *theDialog = NULL;

    /**
     * Iterate through DialogMap and return first occurence of a dialog
     * identified by a name and options.
     */
    DialogMapConstIterator dialogIter = m_dialogList.begin();
    for(;dialogIter != m_dialogList.end(); dialogIter++) {
	if(dialogIter->second->GetName() == dialogName &&
		dialogIter->second->GetWindowTitle() == dialogTitle) {
	    theDialog = dialogIter->second;
	    break;
	}
    }

    return theDialog;
}

ScrollFrameWindow* DialogManager::GetDialog(unsigned int id)
{
    ScrollFrameWindow *theDialog = NULL;

    // Find a dialog in the DialogMap using its unique ID
    DialogMapConstIterator dialogIter = m_dialogList.find(id);
    if(dialogIter != m_dialogList.end())
	theDialog = dialogIter->second;

    return theDialog;
}

void DialogManager::OnReset()
{
    /**
     * Iterate through DialogMap and notify dialogs on Reset event.
     */
    DialogMapConstIterator dialogIter = m_dialogList.begin();
    for(;dialogIter != m_dialogList.end(); dialogIter++)
	dialogIter->second->OnReset();
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
