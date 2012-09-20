#ifndef SCROLL_LISTBOX_H_
#define SCROLL_LISTBOX_H_
/**
 * @file ScrollListbox.h
 * @date 2012-05-10
 * @author fritz@fritzmahnke.com
 *
 */

#include <vector>
#include <string>

#include "ScrollWidget.h"

/**
 *  Base class for a Listbox widget.
 */
class ScrollListbox : public ScrollWidget
{
public:
    /** C-tor */
    explicit ScrollListbox (ScrollFrameWindow *dialog) :
	 ScrollWidget (dialog)
    {
    }

    /** Populate the listbox contents.
     *
     * @param[in] listItems - vector of strings to display in the listbox.
     */
    virtual void Fill (const std::vector<std::string> &listItems) = 0;

protected:
    virtual ~ScrollListbox () { }
};

#endif  // SCROLL_LISTBOX_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
