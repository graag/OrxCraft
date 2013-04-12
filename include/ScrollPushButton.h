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

#ifndef __SCROLL_PUSH_BUTTON_H__
#define __SCROLL_PUSH_BUTTON_H__
/**
 * @file ScrollPushButton.h
 * @date 2012-05-20
 * @author fritz@fritzmahnke.com
 *
 */

#include <string>

#include "ScrollWidget.h"

class ScrollFrameWindow;

/**
 *  Base class for a Push Button widget.
 */
class ScrollPushButton : public ScrollWidget
{
public:
    /** C-tor */
    explicit ScrollPushButton(ScrollFrameWindow *dialog) :
	ScrollWidget(dialog)
    {
    }

    /** Set the text displayed on the Pushbutton */
    virtual void SetText(const std::string& text) = 0;
    /** Not implemented */
    virtual void ConfigRead();
    /** Not implemented */
    virtual void ConfigUpdate();
};

#endif  // __SCROLL_PUSH_BUTTON_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
