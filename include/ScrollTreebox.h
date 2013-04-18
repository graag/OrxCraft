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

#ifndef SCROLL_TREEBOX_H_
#define SCROLL_TREEBOX_H_
/**
 * @file ScrollTreebox.h
 * @date 2013-04-15
 * @author graag@o2.pl
 *
 */

#include <vector>
#include <string>

#include "ScrollWidget.h"

typedef std::pair<std::string, std::string> ScrollTreePair;

/**
 *  Base class for a Tree widget.
 *
 *  Tree widget supports only shallow hierarchy: there is only one level of
 *  groups (no nested subgroups). Such limitation is in accordance with the
 *  implementation of object groups / sets in ScrollEd.
 */
class ScrollTreebox : public ScrollWidget
{
public:
    /** C-tor */
    explicit ScrollTreebox(ScrollFrameWindow *dialog) :
	 ScrollWidget(dialog)
    {
    }

    /** Populate the tree contents.
     *
     * @param[in] treeItems - vector of string pairs (set, item). Set defines
     *     group item belogs to, if it is empty the item will be a top level
     *     leaf of the tree. Items have to be unique for the whole tree.
     */
    virtual void Fill(const std::vector<ScrollTreePair> &treeItems) = 0;
    virtual void SetSelection(const std::vector<std::string> &treeItems) = 0;
    virtual const std::vector<std::string> GetSelection() const = 0;
    /** Set wiget data from orx config for currently selected section.  */
    virtual void ConfigRead();
    /** Set orx config attribute value for currently selected section based on
     * widget data.
     */
    virtual void ConfigUpdate();

protected:
    virtual ~ScrollTreebox () { }
};

#endif  // SCROLL_TREEBOX_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
