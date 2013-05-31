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

#ifndef __CEGUIEXTENDEDTREEITEM_H__
#define __CEGUIEXTENDEDTREEITEM_H__
/**
 * @file CEGUIExtendedTree.h
 * @date 2013-04-16
 * @author graag@o2.pl
 *
 */

#include "CEGUI.h"

/** Extension of CEGUI TreeItem of Tree widget */
class CEGUIExtendedTreeItem : public CEGUI::TreeItem
{
    public:
	/** C-tor */
	CEGUIExtendedTreeItem(const CEGUI::String& text,
		CEGUI::uint item_id = 0, void* item_data = 0,
		bool disabled = false, bool auto_delete = true);

	/** And an item as a child of current item. */
	void addItem(CEGUIExtendedTreeItem* item);
	/** Set an item as a parent for current item. */
	void setParent(CEGUIExtendedTreeItem* item) { d_parent = item; }
	/** Get items parent. */
	CEGUIExtendedTreeItem* getParent(void) { return d_parent; }
	/** Set level of the tree hierarchy the item is stored.
	 * Level 0 corresponds to children of tree root.
	 */
	void setLevel(size_t level) { d_level = level; }
	/** Get level of the tree hierarchy the item is stored.
	 * Level 0 corresponds to children of tree root.
	 */
	size_t getLevel(void) { return d_level; }
	/** Specify if an item is in "open" state. */
	void setIsOpen(bool state) { d_isOpen = state; }

	// Unique widget name throughout CEGUI.
	static CEGUI::String WidgetTypeName;

	/*************************************************************************
	  Operators
	 *************************************************************************/
	/*!
	  \brief
	  Less-than operator, compares item texts. Items with children are
	  considered to be smaller then ones without.
	  */
	virtual bool operator<(const TreeItem& rhs) const;

	/*!
	  \brief
	  Greater-than operator, compares item texts. Items with children are
	  considered to be smaller then ones without.
	  */
	virtual bool operator>(const TreeItem& rhs) const;

    private:
	CEGUIExtendedTreeItem* d_parent;
	size_t d_level;
};
#endif  // __CEGUIEXTENDEDTREEITEM_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
