/**
 * @file CEGUICombobox.cpp
 * @date 2012-05-11
 * @author fritz@fritzmahnke.com
 *
 */

#include "CEGUICombobox.h"
#include "ScrollFrameWindow.h"

using CEGUI::Combobox;
using CEGUI::Event;
using CEGUI::Window;

CEGUICombobox::CEGUICombobox (ScrollFrameWindow *dialog) :
    ScrollCombobox (dialog),
    m_ceCombobox   (NULL)
{
}

void CEGUICombobox::Init (const orxSTRING widgetName)
{
    const orxSTRING windowName = m_manager->GetName ();
    Window *rootWindow = CEGUI::System::getSingleton ().getGUISheet ();
    Window *window = rootWindow->getChild (windowName);

    Combobox *combobox = reinterpret_cast<Combobox *> (
	window->getChild (widgetName));
    combobox->subscribeEvent (Combobox::EventListSelectionAccepted,
	Event::Subscriber (&CEGUICombobox::OnSelectionAccepted, this));

    m_ceCombobox = combobox;
    m_widgetName = new char[strlen (widgetName) + 1];
    strcpy (m_widgetName, widgetName);
}

void CEGUICombobox::Fill (const vector<const orxSTRING> &listItems)
{
    // Iterate through list of items
    for (unsigned int i = 0; i < listItems.size (); i++)
    {
	m_items.push_back (new CEGUI::ListboxTextItem (listItems.at (i)));
	/* Attach the new item to the CEGUI::Combobox. ListboxItems by default
	 * are delated when widget they are attached to is destoyed. Therefore
	 * we should not delete them when m_items vector is destroyed.
	 */
	m_ceCombobox->addItem (m_items.back ());
    }
}

void CEGUICombobox::SelectItem (const orxSTRING text)
{
    orxASSERT (text != orxNULL);

    int i = 0;
    std::vector<CEGUI::ListboxTextItem *>::const_iterator it;
    for (it = m_items.begin (); it != m_items.end (); ++it)
    {
	const orxSTRING itemText = (*it)->getText ().c_str ();
	// Wanted item exists in the items list
	if (orxString_Compare (itemText, text) == 0)
	{
	    // Set the edit box text accordingly
	    m_ceCombobox->setItemSelectState (*it, true);
	    break;
	}
	i++;
    }
    // Text not present in predefined values. Log to debug output.
    if(i >= m_items.size() && orxString_GetLength(text) > 0)
	orxLOG("Item '%s' not found in Combobox '%s'.", text, m_ceCombobox->getName().c_str());
}

const orxSTRING CEGUICombobox::GetSelectedItem () const
{
    CEGUI::ListboxItem *item = m_ceCombobox->getSelectedItem ();
    // Return selected item. The returned string is owned by CEGUI widget.
    return item->getText ().c_str ();
}

bool CEGUICombobox::OnSelectionAccepted (const CEGUI::EventArgs &e)
{
    CEGUI::WindowEventArgs *args = (CEGUI::WindowEventArgs *) &e;
    const orxSTRING widgetName = args->window->getName ().c_str ();
    m_manager->OnTextAccepted (widgetName);

    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
