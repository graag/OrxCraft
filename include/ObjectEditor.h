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
 * @file ObjectEditor.h
 * @date 2012-05-04
 * @author fritz@fritzmahnke.com
 *
 */
#ifndef OBJECTEDITOR_H_
#define OBJECTEDITOR_H_

#include "ScrollFrameWindow.h"

class ScrollObject;
class ScrollCombobox;
class ScrollEditbox;
class ScrollListbox;
class ScrollPushButton;
class ScrollWindowWidget;

class ObjectEditor : public ScrollFrameWindow
{
public:
    /** C-tor */
    ObjectEditor (const std::string& name);

    /** Initialize control items */
    void SetupFields  ();
    /** Update all widgets to show current ScrollObject config values */
    void UpdateFields () const;

    /** Set the ScrollObject being edited */
    void SetObject (ScrollObject *object);
    /** Get the ScrollObject being edited */
    ScrollObject* GetObject();

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
    ScrollObject *m_object;

    ScrollCombobox *m_objConfigName;

    ScrollPushButton  *m_buttonChildList;
    ScrollPushButton  *m_buttonFXList;
};

#endif  // OBJECTEDITOR_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
