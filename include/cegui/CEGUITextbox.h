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

#ifndef __CEGUITEXTBOX_H__
#define __CEGUITEXTBOX_H__
/**
 * @file CEGUITextbox.h
 * @date 2013-04-26
 * @author graag@o2.pl
 *
 */

#include <string>

#include "CEGUI.h"

#include "ScrollTextbox.h"

class ScrollFrameWindow;

/** CEGUI Implementation of a Push Button */
class CEGUITextbox : public ScrollTextbox
{
public:
    /** C-tor */
    explicit CEGUITextbox (ScrollFrameWindow *dialog);
    /** Initialize the widget.
     *
     * @param[in] widgetName - the name will be used to select proper instace
     * of the CEGUI widget
     */
    virtual void Init (const std::string& widgetName);

    /** Set the text displayed on the Pushbutton */
    virtual void SetText (const std::string& text);

private:
    //! Pointer to CEGUI widget
    CEGUI::Window *m_ceStaticText;
};
#endif  // __CEGUITEXTBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
