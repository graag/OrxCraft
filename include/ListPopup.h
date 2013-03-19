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

#include "CEGUI.h"

#include "ScrollFrameWindow.h"

class ScrollListbox;

class ListPopup : public ScrollFrameWindow
{
public:
    ListPopup(const std::string& name, const std::string& title = "");

    virtual void Init ();
    virtual void OnMouseClick   (const std::string& widgetName);
    virtual void OnTextAccepted (const std::string& widgetName);
    virtual void OnPopupFinish  (const std::string& popupName,
	    const std::string& popupTitle = "");
    virtual void OnDestroy ();
    virtual void OnReset () {};
    virtual void Fill (const std::vector<std::string>& dataList);
    virtual void SetSelection (const std::vector<std::string>& selectionList);
    virtual std::vector<std::string> GetSelection();
    virtual void SetUserData(void* data)
    { m_userData = data; }
    virtual void* GetUserData()
    { return m_userData; }
    virtual void SetParent(ScrollFrameWindow* parent)
    { m_parent = parent; }
    virtual void* GetParent()
    { return m_parent; }

private:
    /** Handler for CEGUI::Window::EventCloseClicked event.
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnCloseClicked (const CEGUI::EventArgs &e);

    ScrollListbox*     m_contentList;
    ScrollFrameWindow* m_parent;
    void*              m_userData;
};

#endif	// __LISTPOPUP_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
