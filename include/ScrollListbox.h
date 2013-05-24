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
    explicit ScrollListbox(ScrollFrameWindow *dialog) :
	 ScrollWidget(dialog)
    {
    }

    /** Populate the listbox contents.
     *
     * @param[in] listItems - vector of strings to display in the listbox.
     */
    virtual void Fill(const std::vector<std::string> &listItems) = 0;
    virtual void SetSelection(const std::vector<std::string> &listItems) = 0;
    virtual const std::vector<std::string> GetSelection() const = 0;
    virtual bool HasItem(const std::string& itemName) const = 0;
    /** Set wiget data from orx config for currently selected section.  */
    virtual void ConfigRead();
    /** Set orx config attribute value for currently selected section based on
     * widget data.
     */
    virtual void ConfigUpdate();

protected:
    virtual ~ScrollListbox () { }
};

#endif  // SCROLL_LISTBOX_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
