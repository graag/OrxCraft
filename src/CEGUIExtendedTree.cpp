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

/**
 * @file CEGUIExtendedTree.cpp
 * @date 2013-04-16
 * @author graag@o2.pl
 *
 */

#include "orx.h"

#include "CEGUIExtendedTree.h"

using CEGUI::Tree;
using CEGUI::TreeItem;
using CEGUI::String;
using CEGUI::uint;

CEGUI::String CEGUIExtendedTree::WidgetTypeName = "CEGUIExtendedTree";

CEGUIExtendedTree::CEGUIExtendedTree(
	const String& text, const String& name):
    Tree(text, name)
{
}

/*************************************************************************
    Return the number of selected items in the tree.
*************************************************************************/
size_t CEGUIExtendedTree::getSelectedCount(void) const
{
    size_t itemCount = d_listItems.size();
    size_t count = 0;
    
    for (size_t index = 0; index < itemCount; ++index)
    {
        if (d_listItems[index]->isSelected())
            count++;

        if (d_listItems[index]->getItemCount() > 0)
        {
	    count += getSelectedChildrenCount(d_listItems[index]);
        }
    }
    
    return count;
}

// Recursive!
size_t CEGUIExtendedTree::getSelectedChildrenCount(TreeItem* item) const
{
    size_t itemCount = item->getItemCount();
    TreeItem::LBItemList & itemList = item->getItemList();
    size_t count = 0;
    
    for (size_t index = 0; index < itemCount; ++index)
    {
        if (itemList[index]->isSelected())
            count++;

        if (itemList[index]->getItemCount() > 0)
        {
	    count += getSelectedChildrenCount(itemList[index]);
        }
    }
    
    return count;
}

size_t CEGUIExtendedTree::getSelectedLeavesCount(void) const
{
    size_t itemCount = d_listItems.size();
    size_t count = 0;
    
    for (size_t index = 0; index < itemCount; ++index)
    {
        if (d_listItems[index]->getItemCount() > 0)
        {
	    count += getSelectedChildrenLeavesCount(d_listItems[index]);
        } else if (d_listItems[index]->isSelected()) {
            count++;
	}
    }
    
    return count;
}

// Recursive!
size_t CEGUIExtendedTree::getSelectedChildrenLeavesCount(TreeItem* item) const
{
    size_t itemCount = item->getItemCount();
    TreeItem::LBItemList & itemList = item->getItemList();
    size_t count = 0;
    
    for (size_t index = 0; index < itemCount; ++index)
    {
        if (itemList[index]->getItemCount() > 0)
        {
	    count += getSelectedChildrenCount(itemList[index]);
        } else if (itemList[index]->isSelected()) {
            count++;
	}
    }
    
    return count;
}

// Recursive!
TreeItem* CEGUIExtendedTree::getNextSelectedItemFromList(
	const TreeItem::LBItemList &itemList, const TreeItem* startItem,
	bool& foundStartItem) const
{
    size_t itemCount = itemList.size();
    
    for (size_t index = 0; index < itemCount; ++index)
    {
        if (foundStartItem == true)
        {
            // Already found the startItem, now looking for next selected item.
            if (itemList[index]->isSelected())
                return itemList[index];
        }
        else
        {
            // Still looking for startItem.  Is this it?
            if (itemList[index] == startItem)
                foundStartItem = true;
        }
        
        if (itemList[index]->getItemCount() > 0)
        {
	    TreeItem *foundSelectedTree;
	    foundSelectedTree = getNextSelectedItemFromList(itemList[index]->getItemList(), startItem, foundStartItem);
	    if (foundSelectedTree != 0)
		return foundSelectedTree;
        }
    }
    
    return 0;
}

/*************************************************************************
    Return a pointer to the first selected item.
*************************************************************************/
TreeItem* CEGUIExtendedTree::getFirstSelectedLeaf(void) const
{
    bool found_first = true;
    return getNextSelectedLeafFromList(d_listItems, 0, found_first);
}

/*************************************************************************
    Return a pointer to the next selected item after item 'start_item'
*************************************************************************/
TreeItem* CEGUIExtendedTree::getNextSelectedLeaf(
	const TreeItem* start_item) const
{
    bool found_first = (start_item == 0);
    return getNextSelectedLeafFromList(d_listItems, start_item, found_first);
}

// Recursive!
TreeItem* CEGUIExtendedTree::getNextSelectedLeafFromList(
	const TreeItem::LBItemList &itemList, const TreeItem* startItem,
	bool& foundStartItem) const
{
    size_t itemCount = itemList.size();

    for (size_t index = 0; index < itemCount; ++index)
    {
	if (foundStartItem == true)
	{
	    // Already found the startItem, now looking for next selected item.
	    if (itemList[index]->isSelected() &&
		    itemList[index]->getItemCount() == 0)
		return itemList[index];
	}
	else
	{
	    // Still looking for startItem.  Is this it?
	    if (itemList[index] == startItem)
		foundStartItem = true;
	}

	if (itemList[index]->getItemCount() > 0)
	{
	    TreeItem *foundSelectedTree;
	    foundSelectedTree = getNextSelectedLeafFromList(itemList[index]->getItemList(), startItem, foundStartItem);
	    if (foundSelectedTree != 0)
		return foundSelectedTree;
	}
    }

    return 0;
}

//@TODO override Tree::onMouseButtonDown - Range select, group select

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
