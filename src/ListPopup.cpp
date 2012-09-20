/**
 * @file ListPopup.cpp
 * @date 2012-09-05
 * @author graag@o2.pl
 *
 */
#include "ListPopup.h"

#include <string>

#include "orxCraft.h"

using std::string;

ListPopup::ListPopup(const string& name):
    ScrollFrameWindow(name),
    m_contentList(NULL)
{}

void ListPopup::Init (const string& widgetName)
{
    ScrollFrameWindow::Init(widgetName);
}

void ListPopup::OnMouseClick (const string& widgetName)
{
    orxASSERT (false);
}

void ListPopup::OnTextAccepted (const string& widgetName)
{
    orxASSERT (false);
}

void ListPopup::OnDestroy ()
{
    // This is rather baaaaad :(
    // delete m_window; 
    delete this;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
