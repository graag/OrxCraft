/**
 * @file CEGUIPushButton.cpp
 * @date 2012-05-20
 * @author fritz@fritzmahnke.com
 *
 */

#include "CEGUIPushButton.h"

#include <string>

#include "ScrollFrameWindow.h"

using CEGUI::PushButton;
using CEGUI::Event;
using CEGUI::Window;
using std::string;

CEGUIPushButton::CEGUIPushButton (ScrollFrameWindow *dialog) :
    ScrollPushButton (dialog),
    m_cePushButton   (NULL)
{
}

void CEGUIPushButton::Init (const string& widgetName)
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
    orxASSERT( typeid(*widget) == typeid(PushButton) );
    PushButton *pushbutton = static_cast<PushButton *> (widget);

    // Subscribe to clicked event
    pushbutton->subscribeEvent (PushButton::EventClicked,
	Event::Subscriber (&CEGUIPushButton::OnClicked, this));

    //! @todo Handle right click??

    m_cePushButton = pushbutton;
}

void CEGUIPushButton::SetText (const string& text)
{
    m_cePushButton->setText (text);
}

bool CEGUIPushButton::OnClicked (const CEGUI::EventArgs &e)
{
    /*
     * Static cast will be safe as this handler is connected only to
     * Editbox::EventClicked signal which passes WindowEventArgs struct.
     */
    const CEGUI::WindowEventArgs *args =
    	static_cast<const CEGUI::WindowEventArgs *>( &e );

    string widgetName = args->window->getName().c_str();
    // Pass the event to the ScrollFrameWindow
    m_manager->OnMouseClick (widgetName);

    // Notify that the event was handled
    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
