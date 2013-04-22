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
 * @file CEGUIListbox.cpp
 * @date 2012-05-10
 * @author fritz@fritzmahnke.com
 *
 */

#include "cegui/CEGUIListbox.h"

#include <string>
#include <vector>

#include "ScrollFrameWindow.h"

using CEGUI::Listbox;
using CEGUI::ListboxItem;
using CEGUI::Event;
using CEGUI::Window;
using std::string;
using std::vector;

CEGUIListbox::CEGUIListbox (ScrollFrameWindow *dialog) :
    ScrollListbox (dialog),
    m_ceListbox (NULL)
{
}

void CEGUIListbox::Init (const string& widgetName)
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
    orxASSERT( typeid(*widget) == typeid(Listbox) );
    Listbox *listbox = static_cast<Listbox *> (widget);

    // Subscribe CEGUIListbox to mouse click event
    listbox->subscribeEvent (Window::EventMouseClick,
	Event::Subscriber (&CEGUIListbox::OnMouseClick, this));

    /*
     * Shouldn't we handle other events like EventSelectionChanged? What if a
     * user uses keyboard to access the listbox?
     */
    //! @todo Handle other events

    m_ceListbox = listbox;
}

void CEGUIListbox::Fill (const vector<string> &listItems)
{
    m_ceListbox->resetList();
    m_items.clear();

    for (unsigned int i = 0; i < listItems.size (); i++)
    {
	m_items.push_back (new CEGUI::ListboxTextItem (listItems.at (i)));
	m_items.back()->setSelectionBrushImage(
		"TaharezLook", "ListboxSelectionBrush");
	m_items.back()->setSelectionColours(0x99ff0000);
	/*
	 * Add item to CEGUI::Listbox.
	 * Note that item ownership is passed to CEGUI.
	 */
	m_ceListbox->addItem (m_items.back ());
    }
}

void CEGUIListbox::SetSelection(const vector<string> &listItems)
{
    m_ceListbox->clearAllSelections();

    vector<string>::const_iterator iter;
    for(iter = listItems.begin(); iter != listItems.end(); iter++)
    {
	ListboxItem* item = m_ceListbox->findItemWithText(*iter, NULL);
	if(item != NULL)
	{
	    m_ceListbox->setItemSelectState(item, true);
	}
	else
	{
	    string title = m_manager->GetWindowTitle();
	    if(title.empty())
		title = m_manager->GetName();
	    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		    "Cannot select item '%s' for '%s' from '%s'. Item does not exits.",
		    iter->c_str(), m_widgetName.c_str(), title.c_str());
	}
    }
}

const vector<string> CEGUIListbox::GetSelection() const
{
    size_t count = m_ceListbox->getSelectedCount();
    vector<string> selection;

    if(count == 0)
	return selection;

    ListboxItem* item = m_ceListbox->getFirstSelectedItem();
    selection.push_back(item->getText().c_str());

    for(size_t i=1; i<count; i++)
    {
	item = m_ceListbox->getNextSelected(item);
	selection.push_back(item->getText().c_str());
    }

    return selection;
}

bool CEGUIListbox::OnMouseClick (const CEGUI::EventArgs &e)
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
