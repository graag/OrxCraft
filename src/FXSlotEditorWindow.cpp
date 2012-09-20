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

using std::string;

FXSlotEditorWindow::FXSlotEditorWindow (const string& name) :
    ScrollFrameWindow(name),
    m_fxsType (NULL),
    m_fxsCurve (NULL),
    m_fxsStartTime (NULL),
    m_fxsEndTime (NULL),
    m_fxsStartValue0 (NULL),
    m_fxsStartValue1 (NULL),
    m_fxsStartValue2 (NULL),
    m_fxsEndValue0 (NULL),
    m_fxsEndValue1 (NULL),
    m_fxsEndValue2 (NULL),
    m_fxsPhase (NULL),
    m_fxsPeriod (NULL),
    m_fxsAcceleration (NULL),
    m_fxsAmplification (NULL),
    m_fxsPow (NULL),
    m_fxsAbsolute (NULL),
    m_fxsUseRotation (NULL),
    m_fxsUseScale (NULL),
    m_fxsConfigName (NULL)
{
}

void FXSlotEditorWindow::Init (const string& widgetName)
{
    ScrollFrameWindow::Init(widgetName);

    m_fxsType = FindCombobox ("FXSlotType");
    m_fxsCurve = FindCombobox ("FXSlotCurve");
    m_fxsStartTime = FindEditbox ("FXSlotStartTime");
    m_fxsEndTime = FindEditbox ("FXSlotEndTime");
    m_fxsStartValue0 = FindEditbox ("FXSlotStartValue0");
    m_fxsStartValue1 = FindEditbox ("FXSlotStartValue1");
    m_fxsStartValue2 = FindEditbox ("FXSlotStartValue2");
    m_fxsEndValue0 = FindEditbox ("FXSlotEndValue0");
    m_fxsEndValue1 = FindEditbox ("FXSlotEndValue1");
    m_fxsEndValue2 = FindEditbox ("FXSlotEndValue2");
    m_fxsPhase = FindEditbox ("FXSlotPhase");
    m_fxsPeriod = FindEditbox ("FXSlotPeriod");
    m_fxsAcceleration = FindEditbox ("FXSlotAcceleration");
    m_fxsAmplification = FindEditbox ("FXSlotAmplification");
    m_fxsPow = FindEditbox ("FXSlotPow");
    m_fxsAbsolute = FindCombobox ("FXSlotAbsolute");
    m_fxsUseRotation = FindCombobox ("FXSlotUseRotation");
    m_fxsUseScale = FindCombobox ("FXSlotUseScale");
    m_fxsConfigName = FindCombobox ("FXSlotConfigName");

    SetupFields ();

    SetContext ("FXS-Darken");
}

void FXSlotEditorWindow::SetupFields ()
{
    vector<string> propList;
    propList = OrxCraft::GetInstance ().GetFXSlotList ();
    m_fxsConfigName->Fill (propList);

    orxConfig_PushSection ("FXSlotEditorWindow");

    // Fill combo boxes

    orx_config_util::GetListIntoVector ("Type", propList);
    m_fxsType->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("Curve", propList);
    m_fxsCurve->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("Absolute", propList);
    m_fxsAbsolute->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("UseRotation", propList);
    m_fxsUseRotation->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("UseScale", propList);
    m_fxsUseScale->Fill (propList);

    orxConfig_PopSection ();
}

void FXSlotEditorWindow::UpdateFields () const
{
    char buffer[255];

    // Config name
    m_fxsConfigName->SelectItem (m_context);

    orxConfig_PushSection (m_context.c_str());

    // Type
    const orxSTRING type = orx_config_util::ListToString ("Type");
    m_fxsType->SelectItem (type);
    // Curve
    const orxSTRING curve = orx_config_util::ListToString ("Curve");
    m_fxsCurve->SelectItem (curve);
    // StartTime
    orx_config_util::FloatToString (orxConfig_GetFloat ("StartTime"), buffer);
    m_fxsStartTime->SetText (buffer);
    // EndTime
    orx_config_util::FloatToString (orxConfig_GetFloat ("EndTime"), buffer);
    m_fxsEndTime->SetText (buffer);
    // StartValue
    orx_config_util::VectorToString ("StartValue", 0, buffer);
    m_fxsStartValue0->SetText (buffer);
    orx_config_util::VectorToString ("StartValue", 1, buffer);
    m_fxsStartValue1->SetText (buffer);
    orx_config_util::VectorToString ("StartValue", 2, buffer);
    m_fxsStartValue2->SetText (buffer);
    // EndValue
    orx_config_util::VectorToString ("EndValue", 0, buffer);
    m_fxsEndValue0->SetText (buffer);
    orx_config_util::VectorToString ("EndValue", 1, buffer);
    m_fxsEndValue1->SetText (buffer);
    orx_config_util::VectorToString ("EndValue", 2, buffer);
    m_fxsEndValue2->SetText (buffer);
    // Phase
    orx_config_util::FloatToString (orxConfig_GetFloat ("Phase"), buffer);
    m_fxsPhase->SetText (buffer);
    // Period
    orx_config_util::FloatToString (orxConfig_GetFloat ("Period"), buffer);
    m_fxsPeriod->SetText (buffer);
    // Absolute
    orx_config_util::BoolToString (orxConfig_GetBool ("Absolute"), buffer);
    m_fxsAbsolute->SelectItem (buffer);
    // Acceleration
    orx_config_util::FloatToString (orxConfig_GetFloat ("Acceleration"), buffer);
    m_fxsAcceleration->SetText (buffer);
    // Amplification
    orx_config_util::FloatToString (orxConfig_GetFloat ("Amplification"), buffer);
    m_fxsAmplification->SetText (buffer);
    // Pow
    orx_config_util::FloatToString (orxConfig_GetFloat ("Pow"), buffer);
    m_fxsPow->SetText (buffer);
    // UseRotation
    orx_config_util::BoolToString (orxConfig_GetBool ("UseRotation"), buffer);
    m_fxsUseRotation->SelectItem (buffer);
    // UseScale
    orx_config_util::BoolToString (orxConfig_GetBool ("UseScale"), buffer);
    m_fxsUseScale->SelectItem (buffer);

    orxConfig_PopSection ();
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
	const string& name = m_fxsConfigName->GetSelectedItem ();
	//! @todo Better not to have this in the Scroll singleton
	SetContext (name);
    }

    // Push config section of edited object
    orxConfig_PushSection (m_context.c_str());

    // Update Orx config in memory from value entered in control
    if (widgetName == "FXSlotType")
    {
	const string& type = m_fxsType->GetSelectedItem ();
	orxConfig_SetString ("Type", type.c_str());
    }
    else if (widgetName == "FXSlotCurve")
    {
	const string& curve = m_fxsCurve->GetSelectedItem ();
	orxConfig_SetString ("Curve", curve.c_str());
    }
    else if (widgetName == "FXSlotStartTime")
    {
	orxFLOAT startTime;
	orxString_ToFloat(m_fxsStartTime->GetText().c_str(),
		&startTime, orxNULL);
	orxConfig_SetFloat ("StartTime", startTime);
    }
    else if (widgetName == "FXSlotEndTime")
    {
	orxFLOAT endTime;
	orxString_ToFloat(m_fxsEndTime->GetText().c_str(),
		&endTime, orxNULL);
	orxConfig_SetFloat ("EndTime", endTime);
    }
    else if (widgetName.find("FXSlotStartValue") != string::npos)
    {
	orxFLOAT XFloat;
	orxFLOAT YFloat;
	orxFLOAT ZFloat;
	orxString_ToFloat(m_fxsStartValue0->GetText().c_str(),
		&XFloat, orxNULL);
	orxString_ToFloat(m_fxsStartValue1->GetText().c_str(),
		&YFloat, orxNULL);
	orxString_ToFloat(m_fxsStartValue2->GetText().c_str(),
		&ZFloat, orxNULL);
	orxVECTOR startValue = { {XFloat}, {YFloat}, {ZFloat} };
	orxConfig_SetVector ("StartValue", &startValue);
    }
    else if (widgetName.find("FXSlotEndValue") != string::npos)
    {
	orxFLOAT XFloat;
	orxFLOAT YFloat;
	orxFLOAT ZFloat;
	orxString_ToFloat(m_fxsEndValue0->GetText().c_str(), &XFloat, orxNULL);
	orxString_ToFloat(m_fxsEndValue1->GetText().c_str(), &YFloat, orxNULL);
	orxString_ToFloat(m_fxsEndValue2->GetText().c_str(), &ZFloat, orxNULL);
	orxVECTOR endValue = { {XFloat}, {YFloat}, {ZFloat} };
	orxConfig_SetVector ("EndValue", &endValue);
    }
    else if (widgetName == "FXSlotPhase")
    {
	orxFLOAT phase;
	orxString_ToFloat(m_fxsPhase->GetText().c_str(), &phase, orxNULL);
	orxConfig_SetFloat ("Phase", phase);
    }
    else if (widgetName == "FXSlotPeriod")
    {
	orxFLOAT period;
	orxString_ToFloat(m_fxsPeriod->GetText().c_str(), &period, orxNULL);
	orxConfig_SetFloat ("Period", period);
    }
    else if (widgetName == "FXSlotAbsolute")
    {
	orxBOOL absolute;
	orxString_ToBool(m_fxsAbsolute->GetSelectedItem().c_str(), &absolute,
		          orxNULL);
	orxConfig_SetBool ("Absolute", absolute);
    }
    else if (widgetName == "FXSlotAcceleration")
    {
	orxFLOAT accel;
	orxString_ToFloat(m_fxsAcceleration->GetText().c_str(), &accel,
		orxNULL);
	orxConfig_SetFloat ("Acceleration", accel);
    }
    else if (widgetName == "FXSlotAmplification")
    {
	orxFLOAT ampli;
	orxString_ToFloat(m_fxsAmplification->GetText().c_str(), &ampli,
		orxNULL);
	orxConfig_SetFloat ("Amplification", ampli);
    }
    else if (widgetName == "FXSlotPow")
    {
	orxFLOAT pow;
	orxString_ToFloat(m_fxsPow->GetText().c_str(), &pow, orxNULL);
	orxConfig_SetFloat ("Pow", pow);
    }
    else if (widgetName == "FXSlotUseRotation")
    {
	orxBOOL useRotation;
	orxString_ToBool(m_fxsUseRotation->GetSelectedItem().c_str(),
		&useRotation, orxNULL);
	orxConfig_SetBool ("UseRotation", useRotation);
    }
    else if (widgetName == "FXSlotUseScale")
    {
	orxBOOL useScale;
	orxString_ToBool(m_fxsUseScale->GetSelectedItem().c_str(), &useScale,
			  orxNULL);
	orxConfig_SetBool ("UseScale", useScale);
    }

    orxConfig_PopSection ();

    OrxCraft::GetInstance ().NeedObjectUpdate ();
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
