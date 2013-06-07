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
 * @file CEGUIMultiLineEditbox.cpp
 * @date 2013-06-06
 * @author graag@o2.pl
 *
 */

#include "cegui/CEGUIMultiLineEditbox.h"

#include <string>

#include "orx_config_util.h"

#include "ScrollFrameWindow.h"

using CEGUI::MultiLineEditbox;
using CEGUI::Event;
using CEGUI::Window;
using std::string;

CEGUIMultiLineEditbox::CEGUIMultiLineEditbox (ScrollFrameWindow *dialog) :
    ScrollEditbox (dialog),
    m_ceEditbox   (NULL)
{
}

void CEGUIMultiLineEditbox::Init (const string& widgetName)
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
     * Static cast is now safe as it is guarded by assert. Assert will be
     * active only in debug build so -fno-rtti can be used for release build.
     */
    orxASSERT( typeid(*widget) == typeid(MultiLineEditbox) );
    MultiLineEditbox *editbox = static_cast<MultiLineEditbox *> (widget);

    editbox->subscribeEvent (MultiLineEditbox::EventTextChanged,
	Event::Subscriber (&CEGUIMultiLineEditbox::OnTextAccepted, this));

    //! @todo Handle mouse events.

    m_ceEditbox = editbox;
}

bool CEGUIMultiLineEditbox::OnTextAccepted (const CEGUI::EventArgs &e)
{
#ifdef __orxDEBUG__
    /*
     * Static cast will be safe as this handler is connected only to
     * Editbox::EventTextChanged signal which passes WindowEventArgs struct.
     */
    const CEGUI::WindowEventArgs *args =
    	static_cast<const CEGUI::WindowEventArgs *>( &e );

    string widgetName = args->window->getName().c_str();
    orxASSERT(widgetName == m_widgetUniqueName);
#endif // __orxDEBUG__

    // Pass the event to the ScrollFrameWindow
    m_manager->OnInput(m_widgetName);

    // Notify that the event was handled
    return true;
}

const string CEGUIMultiLineEditbox::GetText ()
{
    // Return Editbox contents.
    return m_ceEditbox->getText ().c_str ();
}

void CEGUIMultiLineEditbox::SetText (const string& text)
{
    m_ceEditbox->setText (text);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
