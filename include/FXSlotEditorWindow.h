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

#ifndef __FXSLOTEDITORWINDOW_H__
#define __FXSLOTEDITORWINDOW_H__
/**
 * @file FXSlotEditorWindow.h
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */

#include <string>

#include "ScrollFrameWindow.h"

class ScrollCombobox;
class ScrollEditbox;
class ScrollListbox;
class ScrollCheckbox;

class FXSlotEditorWindow : public ScrollFrameWindow
{
public:
    /** C-tor */
    FXSlotEditorWindow (const std::string& name);

    /** Initialize control items */
    void SetupFields  ();
    /** Update all fields on the window using current values from config */
    void UpdateFields () const;

    /** Set the FXSlot being edited */
    void SetContext (const std::string& sectionName);
    /** Get the FXSlot being edited */
    const std::string& GetContext();

    /** ScrollFrameWindow interface */

    virtual void Init();
    virtual void OnAction(const std::string& widgetName,
	    const std::string& action = "");
    virtual void OnInput(const std::string& widgetName);
    virtual void OnClose();
    virtual void OnReset();
    virtual void HandlePopup(const std::string& popupName, orxU32 popupID);
    virtual void HandleClose(const std::string& popupName, orxU32 popupID);

private:
    std::string m_context;

    ScrollCombobox *m_fxsConfigName;
};

#endif  // __FXSLOTEDITORWINDOW_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
