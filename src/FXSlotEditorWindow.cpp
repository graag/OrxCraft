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
 * @file FXSlotEditorWindow.cpp
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */

#include "FXSlotEditorWindow.h"

#include <string>

#include "orxCraft.h"
#include "orx_config_util.h"

#include "ScrollCombobox.h"
#include "ScrollEditbox.h"
#include "ScrollCheckbox.h"
#include "DialogManager.h"

using std::string;

FXSlotEditorWindow::FXSlotEditorWindow (const string& name) :
    ScrollFrameWindow(name),
    m_fxsConfigName (NULL)
{
}

void FXSlotEditorWindow::Init ()
{
    FindWidget("FXSlotType")->ConfigBind("Type");
    FindWidget("FXSlotCurve")->ConfigBind("Curve");
    FindWidget("FXSlotStartTime")->ConfigBind(
	    "StartTime", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("FXSlotEndTime")->ConfigBind(
	    "EndTime", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("FXSlotStartValue0")->ConfigBind(
	    "StartValue", orxCRAFT_WIDGET_DATA_VECTOR0);
    FindWidget("FXSlotStartValue1")->ConfigBind(
	    "StartValue", orxCRAFT_WIDGET_DATA_VECTOR1);
    FindWidget("FXSlotStartValue2")->ConfigBind(
	    "StartValue", orxCRAFT_WIDGET_DATA_VECTOR2);
    FindWidget("FXSlotEndValue0")->ConfigBind(
	    "EndValue", orxCRAFT_WIDGET_DATA_VECTOR0);
    FindWidget("FXSlotEndValue1")->ConfigBind(
	    "EndValue", orxCRAFT_WIDGET_DATA_VECTOR1);
    FindWidget("FXSlotEndValue2")->ConfigBind(
	    "EndValue", orxCRAFT_WIDGET_DATA_VECTOR2);
    FindWidget("FXSlotPhase")->ConfigBind(
	    "Phase", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("FXSlotPeriod")->ConfigBind(
	    "Period", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("FXSlotAcceleration")->ConfigBind(
	    "Acceleration", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("FXSlotAmplification")->ConfigBind(
	    "Amplification", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("FXSlotPow")->ConfigBind(
	    "Pow", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("FXSlotAbsolute")->ConfigBind(
	    "Absolute", orxCRAFT_WIDGET_DATA_BOOL);
    FindWidget("FXSlotUseRotation")->ConfigBind(
	    "UseRotation", orxCRAFT_WIDGET_DATA_BOOL);
    FindWidget("FXSlotUseScale")->ConfigBind(
	    "UseScale", orxCRAFT_WIDGET_DATA_BOOL);

    // No Auto ConfigBing for this one
    m_fxsConfigName = FindCombobox ("FXSlotConfigName");
    orxASSERT(m_fxsConfigName != NULL);

    SetupFields ();

    SetContext ("FXS-Darken");
}

void FXSlotEditorWindow::SetupFields()
{
    vector<string> propList;
    propList = OrxCraft::GetInstance().GetFXSlotList();
    m_fxsConfigName->Fill(propList);

    orxConfig_PushSection("FXSlotEditorWindow");

    // Fill combo boxes
    ScrollCombobox* box;

    propList.clear();
    box = FindCombobox("FXSlotType");
    orxASSERT(box != orxNULL);
    orxConf::GetListAsVector("Type", propList);
    box->Fill(propList);

    propList.clear();
    box = FindCombobox("FXSlotCurve");
    orxASSERT(box != orxNULL);
    orxConf::GetListAsVector ("Curve", propList);
    box->Fill (propList);

    orxConfig_PopSection ();
}

void FXSlotEditorWindow::UpdateFields () const
{
    // Config name
    m_fxsConfigName->SetSelection(m_context);

    orxConfig_PushSection(m_context.c_str());

    vector<ScrollWidget *>::const_iterator it;
    for(it = m_widgetList.begin(); it != m_widgetList.end(); it++) {
	if((*it)->GetName() == "FXSlotConfigName")
		continue;
	(*it)->ConfigRead();
    }

    orxConfig_PopSection();
}

const string FXSlotEditorWindow::GetText (const string& widgetName) const
{
    orxASSERT (false);
    return orxNULL;
}

void FXSlotEditorWindow::SetContext (const string& sectionName)
{
    m_context = sectionName;
    UpdateFields ();
}

void FXSlotEditorWindow::OnMouseClick (const string& widgetName)
{
    orxASSERT (false);
}

void FXSlotEditorWindow::OnTextAccepted (const string& widgetName)
{
    if (widgetName == "FXSlotConfigName")
    {
	const string& name = m_fxsConfigName->GetSelection();
	//! @todo Better not to have this in the Scroll singleton
	SetContext (name);
    } else {

	// Push config section of edited object
	orxConfig_PushSection(m_context.c_str());

	// Update Orx config in memory from value entered in control
	ScrollWidget* box = FindWidget(widgetName);
	orxASSERT(box != orxNULL);
	box->ConfigUpdate();

	orxConfig_PopSection();
    }

    OrxCraft::GetInstance().NeedObjectUpdate();
}

void FXSlotEditorWindow::OnPopupFinish (const string& popupName,
	const string& popupTitle)
{
    orxASSERT(false);
}

void FXSlotEditorWindow::OnDestroy ()
{
    OrxCraft::GetInstance().GetDialogManager()->DestroyDialog(m_id);
    /*
     * Beyond this point the dialog was destroyed (delete was issued).
     * Make sure in is not accessed anymore.
     */
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
