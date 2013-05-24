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

#ifndef __CEGUICOMBOBOX_H__
#define __CEGUICOMBOBOX_H__
/**
 * @file CEGUICombobox.h
 * @date 2012-05-11
 * @author fritz@fritzmahnke.com
 *
 */

#include <vector>
#include <string>

#include "CEGUI.h"

#include "ScrollCombobox.h"

class ScrollFrameWindow;

/** CEGUI implementation of a Combobox */
class CEGUICombobox : public ScrollCombobox
{
public:
    /** C-tor */
    explicit CEGUICombobox (ScrollFrameWindow *dialog);
    /** Initialize the widget.
     *
     * @param[in] widgetName - the name will be used to select proper instace
     * of the CEGUI widget
     */
    virtual void Init (const std::string& widgetName);
    /** Populate the listbox contents.
     *
     * @param[in] listItems - vector of strings to display in the listbox.
     */
    virtual void Fill (const std::vector<std::string> &listItems);
    /** Select an item.
     *
     * @param[in] itemText - contents of the item to be selected.
     */
    virtual void SetSelection(const std::string& text);
    /** Get the text of the selected item */
    virtual std::string GetSelection() const;

private:
    /** Handler for CEGUI::Combobox::EventListSelectionAccepted event.
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnSelectionAccepted(const CEGUI::EventArgs &e);

    //! Pointer to CEGUI widget
    CEGUI::Combobox *m_ceCombobox;
    //! List of combobox items
    std::vector<CEGUI::ListboxTextItem *> m_items;
};
#endif  // __CEGUICOMBOBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
