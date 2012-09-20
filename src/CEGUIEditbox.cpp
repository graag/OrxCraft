/**
 * @file CEGUIEditbox.cpp
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */

#include "CEGUIEditbox.h"

#include <string>

#include "ScrollFrameWindow.h"

using CEGUI::Editbox;
using CEGUI::Event;
using CEGUI::Window;
using std::string;

CEGUIEditbox::CEGUIEditbox (ScrollFrameWindow *dialog) :
    ScrollEditbox (dialog),
    m_ceEditbox   (NULL)
{
}

void CEGUIEditbox::Init (const string& widgetName)
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
     * Static cast is now safe as it is guarded by assert. Assert will be
     * active only in debug build so -fno-rtti can be used for release build.
     */
    orxASSERT( typeid(*widget) == typeid(Editbox) );
    Editbox *editbox = static_cast<Editbox *> (widget);

    editbox->subscribeEvent (Editbox::EventTextAccepted,
	Event::Subscriber (&CEGUIEditbox::OnTextAccepted, this));

    //! @todo Handle mouse events.

    m_ceEditbox = editbox;
}

bool CEGUIEditbox::OnTextAccepted (const CEGUI::EventArgs &e)
{
    /*
     * Static cast will be safe as this handler is connected only to
     * Editbox::EventTextAccepted signal which passes WindowEventArgs struct.
     */
    const CEGUI::WindowEventArgs *args =
    	static_cast<const CEGUI::WindowEventArgs *>( &e );

    string widgetName = args->window->getName().c_str();
    // Pass the event to the ScrollFrameWindow
    m_manager->OnTextAccepted (widgetName);

    // Notify that the event was handled
    return true;
}

const string CEGUIEditbox::GetText ()
{
    return m_ceEditbox->getText ().c_str ();
}

void CEGUIEditbox::SetText (const string& text)
{
    m_ceEditbox->setText (text);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
