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

#ifndef __CEGUITREEBOX_H__
#define __CEGUITREEBOX_H__
/**
 * @file CEGUITreebox.h
 * @date 2013-04-15
 * @author graag@o2.pl
 *
 */

#include <string>
#include <vector>
#include <map>

#include "CEGUIExtendedTree.h"
#include "CEGUIExtendedTreeItem.h"

#include "ScrollTreebox.h"

class ScrollFrameWindow;


/** CEGUI Implementation of a Tree */
class CEGUITreebox : public ScrollTreebox
{
public:
    /** C-tor */
    explicit CEGUITreebox (ScrollFrameWindow *dialog);
    /** Initialize the widget.
     *
     * @param[in] widgetName - the name will be used to select proper instace
     * of the CEGUI widget
     */
    virtual void Init(const std::string& widgetName);
    /** Populate the tree contents.
     *
     * @param[in] treeItems - vector of string pairs (set, item). Set defines
     *     group item belogs to, if it is empty the item will be a top level
     *     leaf of the tree. Items have to be unique for the whole tree.
     */
    virtual void Fill(const std::vector<ScrollTreePair> &treeItems);
    virtual void SetSelection(const std::vector<std::string> &treeItems);
    virtual const std::vector<std::string> GetSelection() const;

     //bool OnSelectionChanged (const CEGUI::EventArgs &e);
private:
    /** Handler for CEGUI::Window::EventMouseClick event.
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnMouseClick(const CEGUI::EventArgs &e);

    //! Pointer to CEGUI widget
    CEGUIExtendedTree *m_ceTree;
};
#endif  // __CEGUITREEBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
