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
 * @file InfoWindow.cpp
 * @date 2012-05-04
 * @author fritz@fritzmahnke.com
 *
 */
#include "InfoWindow.h"
#include "orxCraft.h"
#include "DialogManager.h"

void InfoWindow::Init ()
{}

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
    else if (widgetName == "LoadConfig")
    {
	ScrollFrameWindow* dialog =
	    OrxCraft::GetInstance().GetDialogManager()->OpenDialog("FileDialog");
	orxASSERT(dialog != NULL);
    }
}

void InfoWindow::OnTextAccepted (const string& widgetName)
{
    orxASSERT (false);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
