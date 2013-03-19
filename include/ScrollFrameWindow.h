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

#include "Scroll.h"
#include "ScrollWidget.h"

class ScrollEditbox;
class ScrollCombobox;
class ScrollListbox;
class ScrollCheckbox;

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

    //! Initialize the window.
    virtual void Init () = 0;
    /** Get the unique dialog identifier. */
    virtual unsigned int GetId() const { return m_id; }
    /** Get the dialog name */
    virtual const std::string& GetName() { return m_name; }
    virtual const void SetWindowName(const std::string& windowName)
    { m_windowName = windowName; }
    /** Get the name of underlying toolkit window. */
    virtual const std::string& GetWindowName () { return m_windowName; }
    /** Get the window title. */
    virtual const std::string& GetWindowTitle() { return m_title; }
    /** Left mouse click event handler.
     * @param widgetName - name of the widget that originated the event.
     */
    virtual void OnMouseClick   (const std::string& widgetName) = 0;
    /** Text accepted (pressed Enter key) event handler.
     * @param widgetName - name of the widget that originated the event.
     */
    virtual void OnTextAccepted (const std::string& widgetName) = 0;
    virtual void OnPopupFinish  (const std::string& popupName,
	    const std::string& popupTitle = "") = 0;
    virtual void OnDestroy      () = 0;
    virtual void OnReset        () = 0;

    /** Add new widget to the window.
     * @param widget - a pointer to ScrollWidget
     */
    virtual void AddWidget (ScrollWidget *widget);

    //! D-tor
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
    static unsigned int         m_idPool;
    // Unique id of this dialog
    unsigned int                m_id;
    // Widgets contained in the window
    std::vector<ScrollWidget *> m_widgetList;
    // Name of the underlying toolkit window
    std::string                 m_windowName;
    // Name of the dialog
    std::string                 m_name;
    // Options used for creation of the dialog
    std::string                 m_title;
};

#endif  // __SCROLLFRAMEWINDOW_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
