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
 * @file ScrollFrameWindow.cpp
 * @date 2012-07-01
 * @author fritz@fritzmahnke.com
 *
 */

#include "ScrollFrameWindow.h"

#include <string>
#include <typeinfo>

#include "ScrollCombobox.h"
#include "ScrollEditbox.h"
#include "ScrollListbox.h"
#include "ScrollCheckbox.h"

using namespace std;

unsigned int ScrollFrameWindow::m_idPool = 0;

ScrollFrameWindow::ScrollFrameWindow (const string& name,
	const string& options):
    m_id(0),
    m_name(name),
    m_options(options)
{
    // Assign an unique ID
    m_id = m_idPool;
    m_idPool++;
}

ScrollWidget * ScrollFrameWindow::FindWidget (const string& widgetName) const
{
    ScrollWidget *theWidget = NULL;

    /* Iterate through widgets vector.
     * Return first occurence with specified name.
     */
    vector<ScrollWidget *>::const_iterator widgIter;
    for (widgIter = m_widgetList.begin (); widgIter != m_widgetList.end ();
	 ++widgIter)
    {
	if ((*widgIter)->GetName () == widgetName)
	{
	    theWidget = *widgIter;
	    break;
	}
    }

    return theWidget;
}

ScrollEditbox * ScrollFrameWindow::FindEditbox (const string& widgetName) const
{
    ScrollEditbox *theWidget = NULL;
    ScrollWidget *widget = NULL;

    // Find the widget
    widget = FindWidget(widgetName);

    if(widget != NULL)
    {
	theWidget = orxCRAFT_CAST<ScrollEditbox *> (widget);
	orxASSERT( theWidget != NULL );
    }

    return theWidget;
}

ScrollCombobox * ScrollFrameWindow::FindCombobox (const string& widgetName) const
{
    ScrollCombobox *theWidget = NULL;
    ScrollWidget *widget = NULL;

    // Find the widget
    widget = FindWidget(widgetName);

    if(widget != NULL)
    {
	theWidget = orxCRAFT_CAST<ScrollCombobox *> (widget);
	orxASSERT( theWidget != NULL );
    }

    return theWidget;
}

ScrollListbox * ScrollFrameWindow::FindListbox (const string& widgetName) const
{
    ScrollListbox *theWidget = NULL;
    ScrollWidget *widget = NULL;

    // Find the widget
    widget = FindWidget(widgetName);

    if(widget != NULL)
    {
	theWidget = orxCRAFT_CAST<ScrollListbox *> (widget);
	orxASSERT( theWidget != NULL );
    }

    return theWidget;
}

ScrollCheckbox * ScrollFrameWindow::FindCheckbox (
	const string& widgetName) const
{
    ScrollCheckbox *theWidget = NULL;
    ScrollWidget *widget = NULL;

    // Find the widget
    widget = FindWidget(widgetName);

    if(widget != NULL)
    {
	theWidget = orxCRAFT_CAST<ScrollCheckbox *> (widget);
	orxASSERT( theWidget != NULL );
    }

    return theWidget;
}

void ScrollFrameWindow::AddWidget (ScrollWidget *widget)
{
    m_widgetList.push_back (widget);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
