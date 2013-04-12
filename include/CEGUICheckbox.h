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

#ifndef __SCROLL_CEGUICHECKBOX_H__
#define __SCROLL_CEGUICHECKBOX_H__
/**
 * @file CEGUICheckbox.h
 * @date 2012-05-07
 * @author fritz@fritzmahnke.com
 *
 */

#include "ScrollCheckbox.h"

#include <string>

#include "CEGUI.h"

class ScrollFrameWindow;

//! CEGUI implementation of a Checkbox
class CEGUICheckbox : public ScrollCheckbox
{
public:
    /** C-tor */
    explicit CEGUICheckbox (ScrollFrameWindow *manager);
    /** Initialize the widget.
     *
     * @param[in] widgetName - the name will be used to select proper instace
     * of the CEGUI widget
     */
    virtual void Init (const std::string& widgetName);

    /** Set selected status
     *
     * @param[in] select - checkbox is selected (true/false)
     */
    virtual void SetSelection (const orxBOOL select=true);
    /** Get selected status */
    virtual const orxBOOL GetSelection () const;

private:
    /** Handler for CEGUI::Checkbox::EventCheckStateChanged event.
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnCheckStateChanged(const CEGUI::EventArgs &e);

    //! Pointer to CEGUI widget
    CEGUI::Checkbox *m_ceCheckbox;
};
#endif  //  __SCROLL_CEGUICHECKBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
