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
 * @file ProjectBrowser.cpp
 * @date 2013-06-03
 * @author graag@o2.pl
 *
 */

#include "ProjectBrowser.h"

#include <string>

#include "orxCraft.h"
#include "orx_config_util.h"
#include "constants.h"

#include "ScrollCombobox.h"
#include "ScrollEditbox.h"
#include "ScrollCheckbox.h"
#include "DialogManager.h"

using std::string;

ProjectBrowser::ProjectBrowser (const string& name) :
    ScrollFrameWindow(name),
    m_buttonSave(NULL),
    m_buttonLoad(NULL),
    m_buttonNew(NULL)
{
}

void ProjectBrowser::Init ()
{
    FindWidget("PB/PT/ProjectName")->ConfigBind("Name");
    FindWidget("PB/PT/Description")->ConfigBind("Description",
	    orxCRAFT_WIDGET_DATA_STRING_BLOCK);

    // Push buttons
    m_buttonSave = (ScrollPushButton*)FindWidget("PB/PT/SaveButton");
    orxASSERT(m_buttonSave != NULL);
    m_buttonLoad = (ScrollPushButton*)FindWidget("PB/PT/LoadButton");
    orxASSERT(m_buttonLoad != NULL);
    m_buttonNew = (ScrollPushButton*)FindWidget("PB/PT/NewButton");
    orxASSERT(m_buttonNew != NULL);

    SetupFields ();
}

void ProjectBrowser::SetupFields()
{
}

void ProjectBrowser::UpdateFields () const
{
    // @TODO For now only Project name and description is bound to orxConfig
    // Think how widget<->config binding can work with multiple sections in one
    // window
    orxConfig_PushSection(projectSectionName);

    vector<ScrollWidget *>::const_iterator it;
    for(it = m_widgetList.begin(); it != m_widgetList.end(); it++) {
	(*it)->ConfigRead();
    }

    orxConfig_PopSection();
}

void ProjectBrowser::OnAction(const string& widgetName,
	const string& action)
{
    if (widgetName == "PB/PT/ButtonSave")
    {
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER, "Save button pushed.");
    }
    else if (widgetName == "PB/PT/ButtonLoad")
    {
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER, "Load button pushed.");
    }
    else if (widgetName == "PB/PT/ButtonNew")
    {
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER, "New button pushed.");
    }
}

void ProjectBrowser::OnInput(const string& widgetName)
{
    if (widgetName == "PB/PT/Description" || widgetName == "PB/PT/ProjectName")
    {
	// Push config section of OrxProject
	orxConfig_PushSection(projectSectionName);

	// Update Orx config in memory from value entered in control
	ScrollWidget* box = FindWidget(widgetName);
	orxASSERT(box != orxNULL);
	box->ConfigUpdate();

	orxConfig_PopSection ();
    }
}

void ProjectBrowser::HandlePopup (const string& popupName,
	orxU32 popupID)
{
    orxASSERT(false);
}

void ProjectBrowser::HandleClose (const string& popupName,
	orxU32 popupID)
{
    orxASSERT(false);
}

void ProjectBrowser::OnClose()
{
    SignalClose(m_name, m_id);
    OrxCraft::GetInstance().GetDialogManager()->CloseDialog(m_id);
    /*
     * Beyond this point the dialog was destroyed (delete was issued).
     * Make sure in is not accessed anymore.
     */
}

void ProjectBrowser::OnReset()
{
    UpdateFields();
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
