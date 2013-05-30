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

#ifndef __CEGUIEXTENDEDTREE_H__
#define __CEGUIEXTENDEDTREE_H__
/**
 * @file CEGUIExtendedTree.h
 * @date 2013-04-16
 * @author graag@o2.pl
 *
 */

#include "CEGUI.h"
#include "CEGUIExtendedTreeItem.h"

/** Extension of CEGUI Tree widget */
class CEGUIExtendedTree : public CEGUI::Tree
{
    public:
	CEGUIExtendedTree(const CEGUI::String& text,
		const CEGUI::String& name);

	/** Return the number of selected items in the tree.
	 *
	 * @return Total number of attached items that are in the selected
	 * state.
	 */
	size_t getSelectedCount(void) const;
	size_t getSelectedChildrenCount(CEGUI::TreeItem* item) const;
	size_t getSelectedLeavesCount(void) const;
	size_t getSelectedChildrenLeavesCount(CEGUI::TreeItem* item) const;
	/** Return a pointer to the first selected leaf (item without children).
	 *
	 * @return Pointer to a TreeItem based object that is the first
	 * selected item in the tree. Will return 0 if no item is selected.
	 */
	CEGUI::TreeItem* getFirstSelectedLeaf(void) const;
	/** Return a pointer to the next selected item after item \a start_item
	 *
	 * @param[in] start_item  - Pointer to the TreeItem where the search
	 * for the next selected item is to begin.  If this parameter is 0, the
	 * search will begin with the first item in the tree.
	 *
	 * @return Pointer to a TreeItem based object that is the next selected
	 * item in the tree after the item specified by \a start_item. Will
	 * return 0 if no further items were selected.
	 *
	 * @exception InvalidRequestException thrown if \a start_item is not
	 * attached to this tree.
	 */
	CEGUI::TreeItem* getNextSelectedLeaf(
		const CEGUI::TreeItem* start_item) const;

	CEGUI::TreeItem* getNextSelectedItemFromList(
		const CEGUI::TreeItem::LBItemList &itemList,
		const CEGUI::TreeItem* start_item,
		bool& foundStartItem) const;

	CEGUI::TreeItem* getNextSelectedLeafFromList(
		const CEGUI::TreeItem::LBItemList &itemList,
		const CEGUI::TreeItem* start_item,
		bool& foundStartItem) const;

	CEGUI::TreeItem* findItemWithTextFromFlatList(
		const CEGUI::TreeItem::LBItemList &itemList,
		const CEGUI::String& text,
		const CEGUI::TreeItem* startItem,
		bool foundStartItem) const;

	virtual void onMouseButtonDown(CEGUI::MouseEventArgs& e);
	size_t getItemIndex(CEGUI::TreeItem* item);

	CEGUI::TreeItem::LBItemList& getItemList(void)
	{ return d_listItems; }

	// Unique widget name throughout CEGUI.
	static CEGUI::String WidgetTypeName;
};
#endif  // __CEGUIEXTENDEDTREE_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
