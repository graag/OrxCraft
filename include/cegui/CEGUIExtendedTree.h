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
	/** C-tor */
	CEGUIExtendedTree(const CEGUI::String& text,
		const CEGUI::String& name);

	/** Return the number of selected items in the tree.
	 *
	 * @return Total number of attached items that are in the selected
	 * state.
	 */
	size_t getSelectedCount(void) const;
	/** Return the number of selected children of specified item.
	 *
	 * @param[in] item whose children we consider
	 * @return Total number of attached items that are in the selected
	 * state. Search is recursive.
	 */
	size_t getSelectedChildrenCount(CEGUI::TreeItem* item) const;
	/** Return the number of selected leaves in the tree.
	 *
	 * @return Total number of attached items without children (leaves)
	 * that are in the selected state.
	 */
	size_t getSelectedLeavesCount(void) const;
	/** Return the number of selected leaves of specified item.
	 *
	 * @param[in] item whose children we consider
	 * @return Total number of attached items without children (leaves)
	 * that are in the selected state. Search is recursive.
	 */
	size_t getSelectedChildrenLeavesCount(CEGUI::TreeItem* item) const;
	/** Return a pointer to the next selected item after item \a start_item
	 *
	 * @parma[in] itemList - Vector of items that will be searched
	 * recursively
	 *
	 * @param[in] start_item
	 * Pointer to the TreeItem where the search for the next selected item
	 * is to begin.  If this parameter is 0, the search will begin with
	 * the first item in the tree.
	 *
	 * @param[in] foundStartItem - set to true if search for start item
	 * should be skipped
	 *
	 * @return Pointer to a TreeItem based object that is the next
	 * selected item in the tree after the item specified by \a
	 * start_item.  Will return 0 if no further items were selected.
	 *
	 * @exception	InvalidRequestException	thrown if \a start_item is not
	 * attached to this tree.
	 */
	CEGUI::TreeItem* getNextSelectedItemFromList(
		const CEGUI::TreeItem::LBItemList &itemList,
		const CEGUI::TreeItem* start_item,
		bool& foundStartItem) const;
	/** Return a pointer to the first selected leaf (item without children).
	 *
	 * @return Pointer to a TreeItem based object that is the first
	 * selected item in the tree. Will return 0 if no item is selected.
	 */
	CEGUI::TreeItem* getFirstSelectedLeaf(void) const;
	/** Return a pointer to the next selected leaf (item without children)
	 * after item \a start_item
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
	/** Return a pointer to the next selected leaf (item without children)
	 * after item \a start_item
	 *
	 * @parma[in] itemList - Vector of items that will be searched
	 * recursively
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
	CEGUI::TreeItem* getNextSelectedLeafFromList(
		const CEGUI::TreeItem::LBItemList &itemList,
		const CEGUI::TreeItem* start_item,
		bool& foundStartItem) const;
	/** Search item list for an item with the specified text. The list is
	 * treated as flat - children of items in the list are disregarded.
	 *
	 * @param[in] itemList - vector of items to be searched
	 *
	 * @param[in] text
	 * String object containing the text to be searched for.
	 *
	 * @param[in] start_item
	 * TreeItem where the search is to begin, the search will not include
	 * \a item.  If \a item is 0, the search will begin from the first item
	 * in the tree.
	 *
	 * @return Pointer to the first TreeItem in the tree after \a item that
	 * has text matching \a text.  If no item matches the criteria, 0 is
	 * returned.
	 *
	 * @exception InvalidRequestException thrown if \a item is not attached
	 * to this tree.
	 */
	CEGUI::TreeItem* findItemWithTextFromFlatList(
		const CEGUI::TreeItem::LBItemList &itemList,
		const CEGUI::String& text,
		const CEGUI::TreeItem* startItem,
		bool foundStartItem) const;

	/** Return a vector of children items. */
	CEGUI::TreeItem::LBItemList& getItemList(void)
	{ return d_listItems; }

	/** Select a range of items recursively.
	 *
	 * Tree hierarchy is traversed and all items between start and stop are
	 * selected. Order between start and stop can be top-down or bottom-up.
	 *
	 * @param[in] start - Pointer to first item
	 * @param[in] stop - Pointer to last item
	 */
	void selectDeepRange(
		CEGUIExtendedTreeItem* start,
		CEGUIExtendedTreeItem* end);
	/** Select recursively children of an item.
	 *
	 * @param[in] start - Pointer to an item whose children will be
	 * selected
	 * @param[in] stop - Pointer to an item where selection should end.
	 * Stop should not come before start in the tree hierarchy.
	 *
	 * @return true if stop item was found, false otherwise.
	 */
	bool selectDeepRange_children(
		CEGUIExtendedTreeItem* start,
		CEGUIExtendedTreeItem* stop);
	/** Select siblings of an item. Siblings are selected including their
	 * children.
	 *
	 * @param[in] start - Pointer to an item whose siblings will be
	 * selected. Only siblings that come after start in the tree hierarchy
	 * are considered.
	 * @param[in] stop - Pointer to an item where selection should end.
	 * Stop should not come before start in the tree hierarchy.
	 *
	 * @return true if stop item was found, false otherwise.
	 */
	bool selectDeepRange_siblings(
		CEGUIExtendedTreeItem* start,
		CEGUIExtendedTreeItem* end);

	/* Event handlers */
	virtual void onMouseButtonDown(CEGUI::MouseEventArgs& e);

	// Unique widget name throughout CEGUI.
	static CEGUI::String WidgetTypeName;
};
#endif  // __CEGUIEXTENDEDTREE_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
