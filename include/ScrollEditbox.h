#ifndef __SCROLL_EDITBOX_H__
#define __SCROLL_EDITBOX_H__
/**
 * @file ScrollEditbox.h
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */

#include <string>

#include "ScrollWidget.h"

class ScrollFrameWindow;

/**
 *  Base class for an Editbox widget.
 */
class ScrollEditbox : public ScrollWidget
{
public:
    /** C-tor */
    explicit ScrollEditbox (ScrollFrameWindow *dialog) :
	ScrollWidget (dialog)
    {
    }

    /** Get the text contained in the Editbox */
    virtual const std::string GetText () = 0;
    /** Set the text contained in the Editbox */
    virtual void SetText (const std::string& text) = 0;
};

#endif  // __SCROLL_EDITBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
