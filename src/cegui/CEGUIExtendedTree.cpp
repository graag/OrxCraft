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

#include "cegui/CEGUIExtendedTree.h"

#include "orx.h"

using CEGUI::Tree;
using CEGUI::TreeItem;
using CEGUI::String;
using CEGUI::uint;
using CEGUI::MouseEventArgs;
using CEGUI::InvalidRequestException;
using CEGUI::LeftButton;
using CEGUI::Point;
using CEGUI::CoordConverter;
using CEGUI::TreeEventArgs;
using CEGUI::Rect;
using CEGUI::Control;
using CEGUI::Shift;

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

TreeItem* CEGUIExtendedTree::findItemWithTextFromFlatList(
	const TreeItem::LBItemList &itemList,
	const String& text,
	const TreeItem* startItem,
	bool foundStartItem) const
{
    size_t itemCount = itemList.size();
    
    for (size_t index = 0; index < itemCount; ++index)
    {
        if (foundStartItem == true)
        {
            // Already found the startItem, now looking for the actual text.
            if (itemList[index]->getText() == text)
                return itemList[index];
        }
        else
        {
            // Still looking for startItem.  Is this it?
            if (itemList[index] == startItem)
                foundStartItem = true;
        }
    }
    
    return 0;
}

//@TODO override Tree::onMouseButtonDown - Range select, group select
/*************************************************************************
    Handler for when mouse button is pressed
*************************************************************************/
void CEGUIExtendedTree::onMouseButtonDown(MouseEventArgs& e)
{
    // base class processing
    // populateGeometryBuffer();
    Window::onMouseButtonDown(e);
    
    if (e.button == LeftButton)
    {
        bool modified = false;
        
        Point localPos(CoordConverter::screenToWindow(*this, e.position));
        //      Point localPos(screenToWindow(e.position));
        
        TreeItem* item = getItemAtPoint(localPos);
        
        if (item != 0)
        {
            modified = true;
            TreeEventArgs args(this);
            args.treeItem = item;
            populateGeometryBuffer();
            Rect buttonLocation = item->getButtonLocation();
            if ((localPos.d_x >= buttonLocation.d_left) && (localPos.d_x <= buttonLocation.d_right) &&
                (localPos.d_y >= buttonLocation.d_top) && (localPos.d_y <= buttonLocation.d_bottom))
            {
                item->toggleIsOpen();
                if (item->getIsOpen())
                {
                    TreeItem *lastItemInList = item->getTreeItemFromIndex(item->getItemCount() - 1);
                    ensureItemIsVisible(lastItemInList);
                    ensureItemIsVisible(item);
                    onBranchOpened(args);
                }
                else
                {
                    onBranchClosed(args);
                }
                
                // Update the item screen locations, needed to update the scrollbars.
                //	populateGeometryBuffer();
                
                // Opened or closed a tree branch, so must update scrollbars.
                configureScrollbars();
            }
            else
            {
                // clear old selections if no control key is pressed or if multi-select is off
                if (!(e.sysKeys & Control) || !d_multiselect)
                    clearAllSelections_impl();
                
                // select range or item, depending upon keys and last selected item
                if (((e.sysKeys & Shift) && (d_lastSelected != 0)) && d_multiselect)
                    selectRange(getItemIndex(item), getItemIndex(d_lastSelected));
                else
                    item->setSelected(item->isSelected() ^ true);
                
                // update last selected item
                d_lastSelected = item->isSelected() ? item : 0;
                onSelectionChanged(args);
            }
        }
        else
        {
            // clear old selections if no control key is pressed or if multi-select is off
            if (!(e.sysKeys & Control) || !d_multiselect)
            {
                if (clearAllSelections_impl())
                {
                    // Changes to the selections were actually made
                    TreeEventArgs args(this);
                    args.treeItem = item;
                    onSelectionChanged(args);
                }
            }
        }
        
        
        ++e.handled;
    }
}

size_t CEGUIExtendedTree::getItemIndex(TreeItem* item)
{
    // @TODO Search item tree recursively?? Rewrite select range to work on item pointers??
    for(size_t i=0; i<d_listItems.size(); i++)
    {
	if(d_listItems[i] == item)
	    return i;
    }
    CEGUI_THROW(InvalidRequestException(
    		"CEGUIExtendedTree::getItemIndex - the specified TreeItem is "
    		"not attached to this Tree"));
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
