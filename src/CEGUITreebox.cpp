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

#include <string>
#include <vector>

#include "CEGUITreebox.h"

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

typedef map<string, CEGUIExtendedTreeItem*>::iterator mapIter;
typedef multimap<string, CEGUIExtendedTreeItem*>::iterator multimapIter;

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
    CEGUITreePair item;
    CEGUITreePair group;

    // Remove current tree items
    m_ceTree->resetList();

    for (unsigned int i = 0; i < treeItems.size (); i++)
    {
	item.first = treeItems.at(i).first;
	item.second = new CEGUIExtendedTreeItem(treeItems.at(i).second);

	/*
	 * Add item to CEGUI::Tree.
	 * Note that item ownership is passed to CEGUI.
	 */
	if(!item.first.empty()) {
	    multimapIter it = m_items.find(item.first);
	    if(it == m_items.end()) {
		group.first = item.first;
		group.second = new CEGUIExtendedTreeItem(item.first);
		group.second->setSelectionBrushImage(
			"TaharezLook", "ListboxSelectionBrush");
		group.second->setSelectionColours(0x9997c4f0);
		m_groups.insert(group);
	    }
	}

	item.second->setSelectionBrushImage(
		"TaharezLook", "ListboxSelectionBrush");
	item.second->setSelectionColours(0x99ff0000);
	m_items.insert(item);
    }

    multimapIter it;
    mapIter it_group;
    mapIter it_item;
    pair<multimapIter, multimapIter> it_range;
    CEGUIItemMap children;

    for(it_group = m_groups.begin(); it_group != m_groups.end(); it_group++) {
	m_ceTree->addItem(it_group->second);

	children.clear();
	it_range = m_items.equal_range(it_group->first);
	for(it = it_range.first; it != it_range.second; it++) {
	    children.insert(
		    CEGUITreePair(
			string(it->second->getText().c_str()),
			it->second
			)
		    );
	}
	for(it_item = children.begin(); it_item != children.end(); it_item++) {
	    it_group->second->addItem(it_item->second);
	}
    }

    children.clear();
    it_range = m_items.equal_range("");
    for(it = it_range.first; it != it_range.second; it++) {
	children.insert(
		CEGUITreePair(
		    string(it->second->getText().c_str()),
		    it->second
		    )
		);
    }
    for(it_item = children.begin(); it_item != children.end(); it_item++) {
	m_ceTree->addItem(it_item->second);
    }
}

void CEGUITreebox::SetSelection(const vector<string> &listItems)
{
    m_ceTree->clearAllSelections();

    vector<string>::const_iterator iter;
    for(iter = listItems.begin(); iter != listItems.end(); iter++)
    {
    	try {
	    CEGUIExtendedTreeItem* item =
		orxCRAFT_CAST<CEGUIExtendedTreeItem *>(
			m_ceTree->findFirstItemWithText(*iter)
			);
	    item->setSelected(true);
	    if(item->getParent() != NULL)
		// item->getParent()->setIsOpen(true); 
		item->getParent()->toggleIsOpen();
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
    m_manager->OnMouseClick(m_widgetName);

    //! @todo Handle left vs right clicks

    // Notify that the event was handled
    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
