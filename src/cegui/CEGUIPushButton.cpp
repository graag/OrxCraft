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
 * @file CEGUIPushButton.cpp
 * @date 2012-05-20
 * @author fritz@fritzmahnke.com
 *
 */

#include "cegui/CEGUIPushButton.h"

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

void CEGUIPushButton::ConfigRead()
{
    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
	    "CEGUIPushButton::ConfigRead is not implemented.");
    orxASSERT(false);
}

void CEGUIPushButton::ConfigUpdate() const
{
    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
	    "CEGUIPushButton::ConfigUpdate is not implemented.");
    orxASSERT(false);
}

bool CEGUIPushButton::OnClicked (const CEGUI::EventArgs &e)
{
#ifdef __orxDEBUG__
    /*
     * Static cast will be safe as this handler is connected only to
     * Editbox::EventClicked signal which passes WindowEventArgs struct.
     */
    const CEGUI::WindowEventArgs *args =
    	static_cast<const CEGUI::WindowEventArgs *>( &e );

    string widgetName = args->window->getName().c_str();
    orxASSERT(widgetName == m_widgetUniqueName);
#endif // __orxDEBUG__

    // Pass the event to the ScrollFrameWindow
    m_manager->OnAction(m_widgetName);

    // Notify that the event was handled
    return true;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
