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
 * @file CEGUITextBox.cpp
 * @date 2013-04-26
 * @author graag@o2.pl
 *
 */

#include "cegui/CEGUITextbox.h"

#include <string>

#include "ScrollFrameWindow.h"

using CEGUI::Event;
using CEGUI::Window;
using std::string;

CEGUITextbox::CEGUITextbox (ScrollFrameWindow *dialog) :
    ScrollTextbox (dialog),
    m_ceStaticText   (NULL)
{
}

void CEGUITextbox::Init (const string& widgetName)
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

    m_ceStaticText = widget;
}

void CEGUITextbox::SetText (const string& text)
{
    m_ceStaticText->setText (text);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
