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

#ifndef __SCROLLFRAMEWINDOW_H__
#define __SCROLLFRAMEWINDOW_H__
/**
 * @file ScrollFrameWindow.h
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */
#include <vector>
#include <string>

#include <utils/Signal.h>

#include "Scroll.h"
#include "ScrollWidget.h"

class ScrollEditbox;
class ScrollCombobox;
class ScrollListbox;
class ScrollTreebox;
class ScrollCheckbox;
class ScrollPushButton;
class ScrollTextbox;

/**
 *  Interface for a Window.
 */
class ScrollFrameWindow
{
public:
    /** C-tor
     * @param[in] name - name of the dialog
     * @param[in] title - string with dialog title, if not specified the
     *                      dialog name is used as a title. The title
     *                      identifies different instaces of the same dialog
     *                      type.
     */
    ScrollFrameWindow (const std::string& name,
	    const std::string& title = "");

    /** Find a widget by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollWidget pointer if found, NULL otherwise.
     */
    ScrollWidget *FindWidget (const std::string& widgetName) const;
    /** Find a editbox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollEditbox pointer if found, NULL otherwise.
     */
    ScrollEditbox *FindEditbox (const std::string& widgetName) const;
    /** Find a combobox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollCombobox pointer if found, NULL otherwise.
     */
    ScrollCombobox *FindCombobox (const std::string& widgetName) const;
    /** Find a checkbox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollCheckbox pointer if found, NULL otherwise.
     */
    ScrollCheckbox *FindCheckbox (const std::string& widgetName) const;
    /** Find a listbox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollListbox pointer if found, NULL otherwise.
     */
    ScrollListbox *FindListbox (const std::string& widgetName) const;
    /** Find a treebox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollTreebox pointer if found, NULL otherwise.
     */
    ScrollTreebox *FindTreebox (const std::string& widgetName) const;
    /** Find a button by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollPushButton pointer if found, NULL otherwise.
     */
    ScrollPushButton *FindPushButton (const std::string& widgetName) const;
    /** Find a textbox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollTextbox pointer if found, NULL otherwise.
     */
    ScrollTextbox *FindTextbox (const std::string& widgetName) const;

    /** Initialize the window. */
    virtual void Init () = 0;
    /** Get the unique dialog identifier. */
    virtual orxU32 GetId() const { return m_id; }
    /** Get the dialog name */
    virtual const std::string& GetName() { return m_name; }
    /** Set the name of underlying toolkit window. */
    virtual const void SetWindowName(const std::string& windowName)
    { m_windowName = windowName; }
    /** Get the name of underlying toolkit window. */
    virtual const std::string& GetWindowName () { return m_windowName; }
    /** Get the window title. */
    virtual const std::string& GetWindowTitle() { return m_title; }
    /** Store user information within window instance.
     *
     * Used to store context of popup creation, e.g. object and property for
     * which value will be derived from popup selection.
     * @param[in] data - pointer to user specified data structure
     */
    virtual void SetUserData(void* data)
    { m_userData = data; }
    /** Get user information stored in the window instance.
     *
     * @return pointer to user specified data structure
     */
    virtual void* GetUserData()
    { return m_userData; }
    /** Add new widget to the window.
     * @param widget - a pointer to ScrollWidget
     */
    virtual void AddWidget (ScrollWidget *widget);
    /** Check if window was defined as a modal one. */
    bool IsModal() { return m_isModal; }

    /** Callbacks */

    /** Callback for new action from child widget.
     *
     * Used by child widgets to notify about new actions. Button clicks,
     * context menu selections etc.
     * @param widgetName - name of the widget that originated the event.
     * @param action - action executed by the widget.
     */
    virtual void OnAction(const std::string& widgetName,
	    const std::string& action = "") = 0;
    /** Callback for new input from child widget.
     *
     * Used by child widgets to notify about new input: text, numbers,
     * selection etc.
     * @param widgetName - name of the widget that originated the event.
     */
    virtual void OnInput(const std::string& widgetName) = 0;
    /** Window close event handler. */
    virtual void OnClose() = 0;
    /** Project data changed event handler. */
    virtual void OnReset() = 0;

    /** Slots */

    /** Popup finish handler. */
    virtual void HandlePopup(const std::string& popupName, orxU32 popupID) = 0;
    /** Popup closed handler. */
    virtual void HandleClose(const std::string& popupName, orxU32 popupID) = 0;

    /* Signals */

    /** Notify caller on popup close. */
    Gallant::Signal2<const std::string&, orxU32> SignalClose;

    /** D-tor */
    virtual ~ScrollFrameWindow ()
    {
	std::vector<ScrollWidget *>::iterator iter;
	for (iter = m_widgetList.begin ();
	    iter != m_widgetList.end ();
	    ++iter)
	{
	    delete *iter;
	}
	m_widgetList.clear();
    };

protected:
    // Pool of ids. It is incremented whenever new dialog is created.
    static orxU32               m_idPool;
    // Unique id of this dialog
    orxU32                      m_id;
    // Widgets contained in the window
    std::vector<ScrollWidget *> m_widgetList;
    // Name of the underlying toolkit window
    std::string                 m_windowName;
    // Name of the dialog
    std::string                 m_name;
    // Options used for creation of the dialog
    std::string                 m_title;
    // Modal flag
    bool                        m_isModal;
    // User data structure
    void*                       m_userData;
};

#endif  // __SCROLLFRAMEWINDOW_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
