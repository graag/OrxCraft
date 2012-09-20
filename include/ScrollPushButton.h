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
    explicit ScrollPushButton (ScrollFrameWindow *dialog) :
	ScrollWidget (dialog)
    {
    }

    /** Set the text displayed on the Pushbutton */
    virtual void SetText (const std::string& text) = 0;
};

#endif  // __SCROLL_PUSH_BUTTON_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
