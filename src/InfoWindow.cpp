/**
 * @file InfoWindow.cpp
 * @date 2012-05-04
 * @author fritz@fritzmahnke.com
 *
 */
#include "InfoWindow.h"
#include "orxCraft.h"

void InfoWindow::Init (const string& widgetName)
{
    ScrollFrameWindow::Init(widgetName);
}

void InfoWindow::OnMouseClick (const string& widgetName)
{
    if (widgetName == "ObjectSectionList")
    {
	//const orxSTRING itemName = m_widgetManager->GetSelectedItem (widgetName);
    }
    else if (widgetName == "FXSlotSectionList")
    {
	//const orxSTRING itemName = m_widgetManager->GetSelectedItem (widgetName);
    }
    else if (widgetName == "SaveConfig")
    {
	OrxCraft::GetInstance ().SaveEditorConfig ();
    }
}

void InfoWindow::OnTextAccepted (const string& widgetName)
{
    orxASSERT (false);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
