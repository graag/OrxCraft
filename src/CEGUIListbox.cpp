/**
 * @file CEGUIListbox.cpp
 * @date 2012-05-10
 * @author fritz@fritzmahnke.com
 *
 */

#include <string>
#include <vector>

#include "CEGUIListbox.h"

#include "ScrollFrameWindow.h"

using CEGUI::Listbox;
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

    const string windowName = m_manager->GetName();
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
    for (unsigned int i = 0; i < listItems.size (); i++)
    {
	m_items.push_back (new CEGUI::ListboxTextItem (listItems.at (i)));
	/*
	 * Add item to CEGUI::Listbox.
	 * Note that item ownership is passed to CEGUI.
	 */
	m_ceListbox->addItem (m_items.back ());
    }
}

bool CEGUIListbox::OnMouseClick (const CEGUI::EventArgs &e)
{
    /*
     * Static cast will be safe as this handler is connected only to
     * Window::EventMouseClick signal which passes MouseEventArgs struct.
     */
    const CEGUI::WindowEventArgs *args =
    	static_cast<const CEGUI::WindowEventArgs *>( &e );

    string widgetName = args->window->getName().c_str();
    // Pass the event to the ScrollFrameWindow
    m_manager->OnMouseClick(widgetName);

    //! @todo Handle left vs right clicks

    // Notify that the event was handled
    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
