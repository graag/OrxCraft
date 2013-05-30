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
 * @file CEGUITreebox.cpp
 * @date 2013-04-15
 * @author graag@o2.pl
 *
 */

#include "cegui/CEGUITreebox.h"

#include <string>
#include <vector>

#include "ScrollFrameWindow.h"

using CEGUI::Tree;
using CEGUI::TreeItem;
using CEGUI::Event;
using CEGUI::Window;
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::multimap;

CEGUITreebox::CEGUITreebox (ScrollFrameWindow *dialog) :
    ScrollTreebox (dialog),
    m_ceTree (NULL)
{
}

void CEGUITreebox::Init (const string& widgetName)
{
    ScrollWidget::Init(widgetName);

    const string windowName = m_manager->GetWindowName();
    // Get the root window
    Window *rootWindow = CEGUI::System::getSingleton().getGUISheet();
    // Get the parent window. No point in searching all windows.
    Window *window = rootWindow->getChild(windowName);
    // Get recursively the widget, this will handle tabs.
    Window *widget = window->getChildRecursive(widgetName);
    orxASSERT(widget != NULL);

    /*
     * Static cast is now safe as it is guarded by assert. This will be active
     * only in debug build so -fno-rtti can be used for release build
     */
    orxASSERT( typeid(*widget) == typeid(CEGUIExtendedTree) );
    CEGUIExtendedTree *tree = static_cast<CEGUIExtendedTree *> (widget);

    // Subscribe CEGUIListbox to mouse click event
    tree->subscribeEvent (Window::EventMouseClick,
	Event::Subscriber (&CEGUITreebox::OnMouseClick, this));

    /*
     * Shouldn't we handle other events like EventSelectionChanged? What if a
     * user uses keyboard to access the listbox?
     */
    //! @todo Handle other events

    m_ceTree = tree;
}

void CEGUITreebox::Fill (const vector<ScrollTreePair> &treeItems)
{
    // Remove current tree items
    m_ceTree->resetList();

    // Disable sorting. This will speedup the insertion. We have to resort
    // anyway due to our treatment of "group" items (items with children)
    m_ceTree->setSortingEnabled(false);

    // Iterate through item list
    for (unsigned int i = 0; i < treeItems.size (); i++)
    {
	// Item name
	string item_name = treeItems.at(i).second;
	// Full group string: "group1/subgroup1/subsubgroup1"
	string group_full = treeItems.at(i).first;
	// Current group name
	string group_part;
	// Current group item
	CEGUIExtendedTreeItem* group;
	// Previous group item
	CEGUIExtendedTreeItem* parent;
	// Item to insert
	CEGUIExtendedTreeItem* item;
	// Group hierarchy level
	int level = 0;
	// Index of previous separator in full group string
	string::size_type idxStart = 0;
	// Index of current separator in full group string
	string::size_type idxEnd = group_full.find("/", idxStart);
	// Extract current group name
	group_part.assign(group_full, idxStart, idxEnd - idxStart);

	while (idxEnd != string::npos || !group_part.empty())
	{
	    // CEGUI::TreeItem::addItem is not virtual. We have to cast to
	    // proper Item type to auto set parents :(
	    if(level == 0) // Root level
		group = orxCRAFT_CAST<CEGUIExtendedTreeItem *>(
			m_ceTree->findItemWithTextFromFlatList(
			    m_ceTree->getItemList(), group_part, NULL, true)
			);
	    else
		group = orxCRAFT_CAST<CEGUIExtendedTreeItem *>(
			m_ceTree->findItemWithTextFromFlatList(
			    parent->getItemList(), group_part, NULL, true)
			);

	    if(group == NULL) { // Group not found - create it
		group = new CEGUIExtendedTreeItem(group_part);
		// Set selection color
		group->setSelectionBrushImage(
			"TaharezLook", "ListboxSelectionBrush");
		group->setSelectionColours(0x9997c4f0);
		// Attach the group to the tree
		// Note that item ownership is passed to CEGUI.
		if(level == 0)
		    m_ceTree->addItem(group);
		else
		    parent->addItem(group);
		/*
		 TODO Something like this would be nice for adding new groups
		 here is not needed as we add something anyway. However there
		 is no obvious way to make an item hidden ... Maybe use emty
		 one and remove it on first insertion??
		// Dummy item used to mark "group" items during insertion into sorted tree
		TreeItem* group_marker = new CEGUIExtendedTreeItem("");
		// Make the dummy marker item hidden
		group_marker->setDisabled(true);
		// CEGUIExtendedTreeItem for comparison purpose priororitises
		// items with children. As we are adding a "group" item mark it
		// as such by adding dummy child.
		group->addItem(group_marker);
		*/
	    }

	    // Next level
	    parent = group;
	    level++;
	    // Are we at last elemen in the path
	    if(idxEnd != string::npos) { // Not yet search for the next one
		idxStart = ++idxEnd;
		idxEnd = group_full.find("/", idxStart);
		// Extract current group name
		group_part.assign(group_full, idxStart, idxEnd - idxStart);
	    }
	    else // This one was the last one
	    {
		group_part = "";
	    }
	}

	// Create and insert the Item into the tree
	item = new CEGUIExtendedTreeItem(item_name);
	item->setSelectionBrushImage( // Selection colour
		"TaharezLook", "ListboxSelectionBrush");
	item->setSelectionColours(0x99ff0000);
	if(level == 0)
	    m_ceTree->addItem(item);
	else
	    parent->addItem(item);
    }

    // Sort the tree after all insertions. Now we can properly treat all group
    // items as they have their children assigned
    m_ceTree->setSortingEnabled(true);
}

void CEGUITreebox::SetSelection(const vector<string> &listItems)
{
    m_ceTree->clearAllSelections();
    CEGUIExtendedTreeItem* parent = NULL;

    vector<string>::const_iterator iter;
    for(iter = listItems.begin(); iter != listItems.end(); iter++)
    {
    	try {
	    CEGUIExtendedTreeItem* item =
		orxCRAFT_CAST<CEGUIExtendedTreeItem *>(
			m_ceTree->findFirstItemWithText(*iter)
			);
	    item->setSelected(true);
	    parent = item->getParent();
	    while(parent != NULL) {
		parent->setIsOpen(true);
		parent = parent->getParent();
	    }
	} catch(CEGUI::InvalidRequestException &e) {
	    string title = m_manager->GetWindowTitle();
	    if(title.empty())
		title = m_manager->GetName();
	    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		    "Cannot select item '%s' for '%s' from '%s'. Item does not exits.",
		    iter->c_str(), m_widgetName.c_str(), title.c_str());
	}
    }
}

const vector<string> CEGUITreebox::GetSelection() const
{
    size_t count = m_ceTree->getSelectedLeavesCount();
    vector<string> selection;

    if(count == 0)
	return selection;

    TreeItem* item = m_ceTree->getFirstSelectedLeaf();
    orxASSERT(item != NULL)
    selection.push_back(item->getText().c_str());

    for(size_t i=1; i<count; i++)
    {
	item = m_ceTree->getNextSelectedLeaf(item);
	orxASSERT(item != NULL)
	selection.push_back(item->getText().c_str());
    }

    return selection;
}

bool CEGUITreebox::OnMouseClick (const CEGUI::EventArgs &e)
{
#ifdef __orxDEBUG__
    /*
     * Static cast will be safe as this handler is connected only to
     * Window::EventMouseClick signal which passes MouseEventArgs struct.
     */
    const CEGUI::WindowEventArgs *args =
    	static_cast<const CEGUI::WindowEventArgs *>( &e );

    string widgetName = args->window->getName().c_str();
    orxASSERT(widgetName == m_widgetUniqueName);
#endif // __orxDEBUG__

    // Pass the event to the ScrollFrameWindow
    m_manager->OnAction(m_widgetName);

    //! @todo Handle left vs right clicks

    // Notify that the event was handled
    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
