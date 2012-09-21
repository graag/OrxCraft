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
 * @file CEGUICombobox.cpp
 * @date 2012-05-11
 * @author fritz@fritzmahnke.com
 *
 */

#include "CEGUICombobox.h"

#include <vector>
#include <string>
#include <typeinfo>

#include "ScrollFrameWindow.h"

using CEGUI::Combobox;
using CEGUI::Event;
using CEGUI::Window;
using std::string;
using std::vector;

using namespace std;

CEGUICombobox::CEGUICombobox (ScrollFrameWindow *dialog) :
    ScrollCombobox (dialog),
    m_ceCombobox   (NULL)
{
}

void CEGUICombobox::Init (const string& widgetName)
{
    ScrollWidget::Init(widgetName);

    const string& windowName = m_manager->GetName();
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
    orxASSERT( typeid(*widget) == typeid(CEGUI::Combobox) );
    Combobox *combobox = static_cast<Combobox *> (widget);

    // Subscribe to selection accepted event
    combobox->subscribeEvent (Combobox::EventListSelectionAccepted,
	Event::Subscriber (&CEGUICombobox::OnSelectionAccepted, this));

    //! @todo Handle mouse events

    m_ceCombobox = combobox;
}

void CEGUICombobox::Fill (const vector<string> &listItems)
{
    for (unsigned int i = 0; i < listItems.size (); i++)
    {
	m_items.push_back (new CEGUI::ListboxTextItem (listItems.at (i)));
	/*
	 * Add item to CEGUI::Combobox.
	 * Note that item ownership is passed to CEGUI.
	 */
	m_ceCombobox->addItem (m_items.back ());
    }
}

void CEGUICombobox::SelectItem (const string& text)
{
    unsigned int i = 0;
    std::vector<CEGUI::ListboxTextItem *>::const_iterator it;
    for (it = m_items.begin (); it != m_items.end (); ++it)
    {
	string itemText = (*it)->getText ().c_str ();
	// Wanted item exists in the items list
	if (itemText == text)
	{
	    // Set the edit box text accordingly
	    m_ceCombobox->setText (text);
	    break;
	}
	i++;
    }

    // Item does not exist. Make a log entry.
    if(i == m_items.size())
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		"Cannot select item '%s'. Item not present in the Combobox.",
		text.c_str());
}

const string CEGUICombobox::GetSelectedItem () const
{
    string text;
    CEGUI::ListboxItem *item = m_ceCombobox->getSelectedItem ();

    // Found a selected item?
    if(item != NULL)
    	text = item->getText ().c_str ();

    return text;
}

bool CEGUICombobox::OnSelectionAccepted (const CEGUI::EventArgs &e)
{
    /*
     * Static cast will be safe as this handler is connected only to
     * Combobox::EventListSelectionAccepted signal which passes MouseEventArgs
     * struct.
     */
    const CEGUI::WindowEventArgs *args =
    	static_cast<const CEGUI::WindowEventArgs *>( &e );

    string widgetName = args->window->getName ().c_str ();
    // Pass the event to the ScrollFrameWindow
    m_manager->OnTextAccepted (widgetName);

    // Notify that the event was handled
    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
