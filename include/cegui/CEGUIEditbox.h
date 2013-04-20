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

#ifndef __SCROLL_CEGUIEDITBOX_H__
#define __SCROLL_CEGUIEDITBOX_H__
/**
 * @file CEGUIEditbox.h
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */

#include <string>

#include "CEGUI.h"

#include "ScrollEditbox.h"

class ScrollFrameWindow;

/** CEGUI Implementation of an Editbox */
class CEGUIEditbox : public ScrollEditbox
{
public:
    /** C-tor */
    explicit CEGUIEditbox (ScrollFrameWindow *dialog);

    /** Initialize the widget.
     * @param[in] widgetName - the name will be used to select proper instace
     * of the CEGUI widget
     */
    virtual void Init (const std::string& widgetName);

    /** Get the text contained in the Editbox */
    virtual const std::string GetText();
    /** Set the text contained in the Editbox */
    virtual void SetText(const std::string& text);

private:
    /** Handler for CEGUI::Editbox::EventTextAccepted event
     * (pressed Enter key).
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnTextAccepted(const CEGUI::EventArgs &e);

    //! Pointer to CEGUI widget
    CEGUI::Editbox *m_ceEditbox;
};
#endif  // __SCROLL_CEGUIEDITBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
