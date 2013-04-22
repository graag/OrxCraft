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
 * @file ListPopup.h
 * @date 2012-09-05
 * @author graag@o2.pl
 *
 */
#ifndef __LISTPOPUP_H__
#define __LISTPOPUP_H__

#include <string>

#include "ScrollFrameWindow.h"

class ScrollListbox;

class ListPopup : public ScrollFrameWindow
{
public:
    /** C-tor */
    ListPopup(const std::string& name, const std::string& title = "");

    /** Fill list widget contents.
     *
     * @param[in] dataList - Vector of list element names
     */
    virtual void Fill (const std::vector<std::string>& dataList);
    /** Set selected list elements.
     *
     * @param[in] selectionlist - vector of list element names to select
     */
    virtual void SetSelection (const std::vector<std::string>& selectionList);
    /** Get selected list elements.
     *
     * @return Vector of selected list element names
     */
    virtual std::vector<std::string> GetSelection();

    /* ScrollFrameWindow interface */

    virtual void OnAction(const std::string& widgetName,
	    const std::string& action = "");
    virtual void OnInput(const std::string& widgetName);
    virtual void OnClose();
    virtual void OnReset() { OnClose(); }
    virtual void HandlePopup(const std::string& popupName, orxU32 popupID);
    virtual void HandleClose(const std::string& popupName, orxU32 popupID);

    /* Signals */

    /** Notify caller on popup finish. */
    Gallant::Signal2<const std::string&, orxU32> SignalFinish;

protected:
    // Listbox widget
    ScrollListbox*     m_contentList;
};

#endif	// __LISTPOPUP_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
