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
#include <algorithm>

/*
 * For debug build perform a dynamic_cast.
 * Remember to check that result != NULL.
 * For release build static_cast is used.
 */
#ifdef __orxDEBUG__
#define orxCRAFT_CAST dynamic_cast
#else
#define orxCRAFT_CAST static_cast
#endif

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

// Used to override binding of Tree widgets specified in layout to a Class
// implementing it.
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

    // Count selected items recursively (original implementation consideres
    // only root children)
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
// Implementation for recursive getSelectedCount - missing from orignal Tree
size_t CEGUIExtendedTree::getSelectedChildrenCount(TreeItem* item) const
{
    size_t itemCount = item->getItemCount();
    TreeItem::LBItemList & itemList = item->getItemList();
    size_t count = 0;

    for (size_t index = 0; index < itemCount; ++index)
    {
	// Count all selected items also groups/subgroups
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
        // Only count items without children
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
        // Only count items without children
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
// Count also closed items not only open (contrary to the original)
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
    Return a pointer to the first selected leaf.
*************************************************************************/
TreeItem* CEGUIExtendedTree::getFirstSelectedLeaf(void) const
{
    bool found_first = true;
    return getNextSelectedLeafFromList(d_listItems, 0, found_first);
}

/*************************************************************************
    Return a pointer to the next selected leaf after item 'start_item'
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
	    // Only consider items without children
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

    // Do not recurse into the children

    return 0;
}

/*************************************************************************
    Handler for when mouse button is pressed

    Implement range select with Shift
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
		{
		    // To many non virtual methods in TreeItem - cast to proper
		    // types
		    CEGUIExtendedTreeItem* start =
			orxCRAFT_CAST<CEGUIExtendedTreeItem *>(item);
		    CEGUIExtendedTreeItem* stop =
			orxCRAFT_CAST<CEGUIExtendedTreeItem *>(d_lastSelected);
                    selectDeepRange(start, stop);
		}
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

void CEGUIExtendedTree::selectDeepRange(
	CEGUIExtendedTreeItem* start,
	CEGUIExtendedTreeItem* stop)
{
    // First we have to determine if the selection is top-down or bottom-up

    CEGUIExtendedTreeItem* deep; // Item at deeper level of tree hierarchy
    CEGUIExtendedTreeItem* shallow; // Item at less deep level of tree hierarchy
    // Parents will be searched to a level where deep an shallow have commot
    // grand parent
    CEGUIExtendedTreeItem* deep_parent; // Parent of the deep item
    CEGUIExtendedTreeItem* shallow_parent; // Parent of the shallow item
    CEGUIExtendedTreeItem* deep_proxy; // Child of deep_parent that contains deep item
    CEGUIExtendedTreeItem* shallow_proxy; // Child of shallow_parent that contains shallow item
    size_t deep_level; // level of tree hierarchy at which deep item is located
    size_t shallow_level; // level of tree hierarchy at which shallow item is located

    // Assign the deep and shallow labels
    if(start->getLevel() >= stop->getLevel())
    {
	deep = start;
	shallow = stop;
    }
    else
    {
	deep = stop;
	shallow = start;
    }

    // Search for parents of deep and shallow that are at the same tree level
    shallow_parent = shallow->getParent();
    shallow_level = shallow->getLevel();
    deep_proxy = deep;
    deep_level = deep->getLevel();

    while(deep_level > shallow_level)
    {
	deep_proxy = deep_proxy->getParent();
	deep_level = deep_proxy->getLevel();
    }
    deep_parent = deep_proxy->getParent();

    // Deep is a child of shallow
    if(shallow == deep_proxy) {
	start = shallow;
	stop = deep;
    }
    else
    {
	// Deep and shallow are more like siblings
	// Search for common ancestor
	shallow_proxy = shallow;
	while(shallow_parent != deep_parent) {
	    shallow_proxy = shallow_parent;
	    deep_proxy = deep_parent;
	    // both are at the same level - go up the tree simultanously
	    shallow_parent = shallow_parent->getParent();
	    deep_parent = deep_parent->getParent();
	}

	size_t deep_idx; // Idx of deep parent (in grand parents children list)
	size_t shallow_idx; // Idx of shallow parent (in grand parents children list)
	TreeItem::LBItemList* item_list;

	// Get grand parents children list. If grand parent is NULL this
	// corresponds to root of the tree. Ask the tree directly for the item
	// list.
	if(deep_parent == NULL)
	{
	    item_list = &getItemList();
	}
	else
	{
	    item_list = &deep_parent->getItemList();
	}

	// Search for the indexes in the item list
	for(size_t i = 0; i<item_list->size(); i++)
	{
	    if((*item_list)[i] == deep_proxy)
		deep_idx = i;
	    if((*item_list)[i] == shallow_proxy)
		shallow_idx = i;
	}

	// Determine the order of shallow and deep
	if(shallow_idx <= deep_idx)
	{
	    start = shallow;
	    stop = deep;
	}
	else
	{
	    start = deep;
	    stop = shallow;
	}
    }

    // Select "start" children. If "stop" is among them stop selection
    if(selectDeepRange_children(start, stop))
	return;

    // Select "start" siblings until "stop" is found.
    while(start != NULL)
    {
	if(selectDeepRange_siblings(start, stop))
	    return;

	start = start->getParent();
    }
}

bool CEGUIExtendedTree::selectDeepRange_children(
	CEGUIExtendedTreeItem* start,
	CEGUIExtendedTreeItem* stop)
{
    // Select the item
    start->setSelected(true);
    // Item is the last one in the selection
    if(start == stop)
	return true;

    if(start->getItemCount())
    {
	// Iterate over children
	TreeItem::LBItemList::iterator it = start->getItemList().begin();
	for( ; it != start->getItemList().end(); it++)
	{
	    // Select the child with grandchildren
	    CEGUIExtendedTreeItem* item =
		orxCRAFT_CAST<CEGUIExtendedTreeItem *>(*it);
	    if(selectDeepRange_children(item, stop))
		return true; // Stop item was found among children
	}
    }

    return false;
}

bool CEGUIExtendedTree::selectDeepRange_siblings(
	CEGUIExtendedTreeItem* start,
	CEGUIExtendedTreeItem* stop)
{
    // Get parent and its list of children - those are our siblings
    CEGUIExtendedTreeItem* parent = start->getParent();
    TreeItem::LBItemList* item_list;

    // Null parent corresponds to root of the tree - get item list directly
    // from the tree
    if(parent == NULL)
    {
	item_list = &getItemList();
    }
    else
    {
	item_list = &parent->getItemList();
    }

    // Find start item in the children list - we'll start selection from next
    // item in the list
    TreeItem::LBItemList::iterator it =
	std::find(item_list->begin(), item_list->end(), start);
    it++;
    // Iterate over siblings
    for( ; it != item_list->end(); it++)
    {
	// Select the sibling and its children
	CEGUIExtendedTreeItem* item =
	    orxCRAFT_CAST<CEGUIExtendedTreeItem *>(*it);
	if(selectDeepRange_children(item, stop))
	    return true; // Stop item was found as one of the children
    }

    return false;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
