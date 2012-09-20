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

using namespace std;

ScrollFrameWindow::ScrollFrameWindow (const string& name):
    m_name(name)
{
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

void ScrollFrameWindow::AddWidget (ScrollWidget *widget)
{
    m_widgetList.push_back (widget);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
