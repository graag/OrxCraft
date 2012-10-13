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
 * @file ObjectEditor.cpp
 * @date 2012-05-04
 * @author fritz@fritzmahnke.com
 *
 */
#include "ObjectEditor.h"

#include <string>
#include <vector>

#include "orxCraft.h"
#include "orx_config_util.h"
#include "orxcraft_util.h"

#include "DialogManager.h"
#include "ScrollCombobox.h"
#include "ScrollEditbox.h"
#include "ScrollPushButton.h"
#include "CEDialogManager.h"
#include "ListPopup.h"

using std::string;
using std::vector;

typedef struct {
    ScrollObject* object;
    string        property;
} PopupData;


ObjectEditor::ObjectEditor (const string& name) :
    ScrollFrameWindow(name),
    m_object (NULL),
    m_objConfigName (NULL),
    m_objAlpha (NULL),
    m_objAngVelocity (NULL),
    m_objPosX (NULL),
    m_objPosY (NULL),
    m_objPosZ (NULL),
    m_objFXList (NULL),
    m_objChildList (NULL),
    m_objAnimFreq (NULL),
    m_objAnimSet (NULL),
    m_objAutoScroll (NULL),
    m_objBlendMode (NULL),
    m_objBody (NULL),
    m_objChildJointList (NULL),
    m_objClock (NULL),
    m_objDepthScale (NULL),
    m_objGraphic (NULL),
    m_objFlip (NULL),
    m_objLifeTime (NULL),
    m_objParentCam (NULL),
    m_objRepeatX (NULL),
    m_objRepeatY (NULL),
    m_objRepeatZ (NULL),
    m_objRotation (NULL),
    m_objScaleX (NULL),
    m_objScaleY (NULL),
    m_objScaleZ (NULL),
    m_objSpeedX (NULL),
    m_objSpeedY (NULL),
    m_objSpeedZ (NULL),
    m_objSmoothing (NULL),
    m_objShaderList (NULL),
    m_objSoundList (NULL),
    m_objSpawner (NULL),
    m_objUseParentSpace (NULL),
    m_objUseRelativeSpeed (NULL),
    m_objColorR (NULL),
    m_objColorG (NULL),
    m_objColorB (NULL),
    m_buttonChildList(NULL)
{
}

void ObjectEditor::Init ()
{
    m_objConfigName = FindCombobox ("ObjectConfigName");
    m_objAlpha = FindEditbox ("ObjAlpha");
    m_objAngVelocity = FindEditbox ("ObjAngularVelocity");
    m_objPosX = FindEditbox ("ObjPos0");
    m_objPosY = FindEditbox ("ObjPos1");
    m_objPosZ = FindEditbox ("ObjPos2");
    m_objFXList = FindEditbox ("ObjFXList");
    m_objChildList = FindEditbox ("ObjChildList");
    m_objAnimFreq = FindEditbox ("ObjAnimFreq");
    m_objAnimSet = FindEditbox ("ObjAnimSet");
    m_objAutoScroll = FindCombobox ("ObjAutoScroll");
    m_objBlendMode = FindCombobox ("ObjBlendMode");
    m_objBody = FindEditbox ("ObjBody");
    m_objChildJointList = FindListbox ("ObjChildJointList");
    m_objClock = FindEditbox ("ObjClock");
    m_objDepthScale = FindCombobox ("ObjDepthScale");
    m_objGraphic = FindCombobox ("ObjGraphic");
    m_objFlip = FindCombobox ("ObjFlip");
    m_objLifeTime = FindEditbox ("ObjLifeTime");
    m_objParentCam = FindEditbox ("ObjParentCam");
    m_objRepeatX = FindEditbox ("ObjRepeat0");
    m_objRepeatY = FindEditbox ("ObjRepeat1");
    m_objRepeatZ = FindEditbox ("ObjRepeat2");
    m_objRotation = FindEditbox ("ObjRotation");
    m_objScaleX = FindEditbox ("ObjScale0");
    m_objScaleY = FindEditbox ("ObjScale1");
    m_objScaleZ = FindEditbox ("ObjScale2");
    m_objSpeedX = FindEditbox ("ObjSpeedX");
    m_objSpeedY = FindEditbox ("ObjSpeedY");
    m_objSpeedZ = FindEditbox ("ObjSpeedZ");
    m_objSmoothing = FindCombobox ("ObjSmoothing");
    m_objShaderList = FindEditbox ("ObjShaderList");
    m_objSoundList = FindEditbox ("ObjSoundList");
    m_objSpawner = FindEditbox ("ObjSpawner");
    m_objUseParentSpace = FindCombobox ("ObjUseParentSpace");
    m_objUseRelativeSpeed = FindCombobox ("ObjUseRelativeSpeed");
    m_objColorR = FindEditbox ("ObjColor0");
    m_objColorG = FindEditbox ("ObjColor1");
    m_objColorB = FindEditbox ("ObjColor2");
    m_buttonChildList = (ScrollPushButton*)FindWidget("ButtonChildList");
    m_buttonFXList = (ScrollPushButton*)FindWidget("ButtonFXList");

    orxASSERT(m_objConfigName != NULL);
    orxASSERT(m_objAlpha != NULL);
    orxASSERT(m_objAngVelocity != NULL);
    orxASSERT(m_objPosX != NULL);
    orxASSERT(m_objPosY != NULL);
    orxASSERT(m_objPosZ != NULL);
    orxASSERT(m_objFXList != NULL);
    orxASSERT(m_objChildList != NULL);
    orxASSERT(m_objAnimFreq != NULL);
    orxASSERT(m_objAnimSet != NULL);
    orxASSERT(m_objAutoScroll != NULL);
    orxASSERT(m_objBlendMode != NULL);
    orxASSERT(m_objBody != NULL);
    orxASSERT(m_objChildJointList != NULL);
    orxASSERT(m_objClock != NULL);
    orxASSERT(m_objDepthScale != NULL);
    orxASSERT(m_objGraphic != NULL);
    orxASSERT(m_objFlip != NULL);
    orxASSERT(m_objLifeTime != NULL);
    orxASSERT(m_objParentCam != NULL);
    orxASSERT(m_objRepeatX != NULL);
    orxASSERT(m_objRepeatY != NULL);
    orxASSERT(m_objRepeatZ != NULL);
    orxASSERT(m_objRotation != NULL);
    orxASSERT(m_objScaleX != NULL);
    orxASSERT(m_objScaleY != NULL);
    orxASSERT(m_objScaleZ != NULL);
    orxASSERT(m_objSpeedX != NULL);
    orxASSERT(m_objSpeedY != NULL);
    orxASSERT(m_objSpeedZ != NULL);
    orxASSERT(m_objSmoothing != NULL);
    orxASSERT(m_objShaderList != NULL);
    orxASSERT(m_objSoundList != NULL);
    orxASSERT(m_objSpawner != NULL);
    orxASSERT(m_objUseParentSpace != NULL);
    orxASSERT(m_objUseRelativeSpeed != NULL);
    orxASSERT(m_objColorR != NULL);
    orxASSERT(m_objColorG != NULL);
    orxASSERT(m_objColorB != NULL);
    orxASSERT(m_buttonChildList != NULL);

    SetupFields ();
}

void ObjectEditor::SetupFields ()
{
    vector<string> propList =
	OrxCraft::GetInstance ().GetObjectList ();

    m_objConfigName->Fill (propList);

    propList = OrxCraft::GetInstance ().GetGraphicList ();

    m_objGraphic->Fill (propList);

    orxConfig_PushSection ("ObjectEditor");

    // Fill combo boxes
    propList.clear ();
    orx_config_util::GetListIntoVector ("AutoScroll", propList);
    m_objAutoScroll->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("BlendMode", propList);
    m_objBlendMode->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("DepthScale", propList);
    m_objDepthScale->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("Flip", propList);
    m_objFlip->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("Smoothing", propList);
    m_objSmoothing->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("UseParentSpace", propList);
    m_objUseParentSpace->Fill (propList);

    propList.clear ();
    orx_config_util::GetListIntoVector ("UseRelativeSpeed", propList);
    m_objUseRelativeSpeed->Fill (propList);

    orxConfig_PopSection ();

    ScrollObject *object =
	OrxCraft::GetInstance ().GetObjectByName ("O-Soldier");

    SetObject (object);
}

void ObjectEditor::SetObject (ScrollObject *object)
{
    m_object = object;
    UpdateFields ();
}

ScrollObject* ObjectEditor::GetObject()
{
    return m_object;
}

void ObjectEditor::UpdateFields () const
{
    if (m_object != orxNULL)
    {
	char buffer[255];

	const orxSTRING name = m_object->GetModelName ();

	// Config name
	m_objConfigName->SelectItem (name);

	orxConfig_PushSection (name);

	// Alpha
	orx_config_util::FloatToString (orxConfig_GetFloat ("Alpha"), buffer);
	m_objAlpha->SetText (buffer);
	// AngularVelocity
	orx_config_util::FloatToString (orxConfig_GetFloat ("AngularVelocity"), buffer);
	m_objAngVelocity->SetText (buffer);
	// AnimationFrequency
	orx_config_util::FloatToString (orxConfig_GetFloat ("AnimationFrequency"), buffer);
	m_objAnimFreq->SetText (buffer);
	// AnimationSet
	const string& aS = orx_config_util::ListToString ("AnimationSet");
	m_objAnimSet->SetText (aS);
	// AutoScroll
	vector<string> aSc;
	orx_config_util::GetListIntoVector ("AutoScroll", aSc);
	m_objAutoScroll->Fill(aSc);
	// BlendMode
	vector<string> bl;
	orx_config_util::GetListIntoVector ("BlendMode", bl);
	m_objBlendMode->Fill(bl);
	// Body
	const string& body = orx_config_util::ListToString ("Body");
	m_objBody->SetText (body);
	// ChildList
	const string& children = orx_config_util::ListToString("ChildList");
	m_objChildList->SetText(children);
	// ChildJointList
	/** @todo ChildJointList */
	// Clock
	const string& clock = orx_config_util::ListToString ("Clock");
	m_objClock->SetText (clock);
	// Color
	orx_config_util::VectorToString ("Color", 0, buffer);
	m_objColorR->SetText (buffer);
	orx_config_util::VectorToString ("Color", 1, buffer);
	m_objColorG->SetText (buffer);
	orx_config_util::VectorToString ("Color", 2, buffer);
	m_objColorB->SetText (buffer);
	// DepthScale
	orx_config_util::BoolToString (orxConfig_GetBool ("DepthScale"), buffer);
	m_objDepthScale->SelectItem (buffer);
	// Graphic
	const string& graphic = orx_config_util::ListToString ("Graphic");
	m_objGraphic->SelectItem (graphic);
	// Flip
	const string& flip = orx_config_util::ListToString ("Flip");
	m_objFlip->SelectItem (flip);
	// FXList
	const string& fxList = orx_config_util::ListToString ("FXList");
	m_objFXList->SetText (fxList);
	// LifeTime
	orx_config_util::FloatToString (orxConfig_GetFloat ("LifeTime"), buffer);
	m_objLifeTime->SetText (buffer);
	// ParentCamera
	const string& parentCam = orx_config_util::ListToString ("ParentCamera");
	m_objParentCam->SetText (parentCam);
	// Position
	orx_config_util::VectorToString ("Position", 0, buffer);
	m_objPosX->SetText (buffer);
	orx_config_util::VectorToString ("Position", 1, buffer);
	m_objPosY->SetText (buffer);
	orx_config_util::VectorToString ("Position", 2, buffer);
	m_objPosZ->SetText (buffer);
	// Repeat
	orx_config_util::VectorToString ("Repeat", 0, buffer);
	m_objRepeatX->SetText (buffer);
	orx_config_util::VectorToString ("Repeat", 1, buffer);
	m_objRepeatY->SetText (buffer);
	orx_config_util::VectorToString ("Repeat", 2, buffer);
	m_objRepeatZ->SetText (buffer);
	// Rotation
	orx_config_util::FloatToString (orxConfig_GetFloat ("Rotation"), buffer);
	m_objRotation->SetText (buffer);
	// Speed
	orx_config_util::VectorToString ("Speed", 0, buffer);
	m_objSpeedX->SetText (buffer);
	orx_config_util::VectorToString ("Speed", 1, buffer);
	m_objSpeedY->SetText (buffer);
	orx_config_util::VectorToString ("Speed", 2, buffer);
	m_objSpeedZ->SetText (buffer);
	// Scale
	orx_config_util::VectorToString ("Scale", 0, buffer);
	m_objScaleX->SetText (buffer);
	orx_config_util::VectorToString ("Scale", 1, buffer);
	m_objScaleY->SetText (buffer);
	orx_config_util::VectorToString ("Scale", 2, buffer);
	m_objScaleZ->SetText (buffer);
	// ShaderList
	//! @todo ShaderList
	// SoundList
	//! @todo SoundList
	// Spawner
	const string& spawner = orx_config_util::ListToString ("Spawner");
	m_objSpawner->SetText (spawner);
	// Smoothing
	orx_config_util::BoolToString (orxConfig_GetBool ("Smoothing"), buffer);
	m_objSmoothing->SelectItem (buffer);
	// UseParentSpace
	orx_config_util::BoolToString (orxConfig_GetBool ("UseParentSpace"),
				       buffer);
	m_objUseParentSpace->SelectItem (buffer);
	// UseRelativeSpeed
	orx_config_util::BoolToString (orxConfig_GetBool ("UseRelativeSpeed"),
				       buffer);
	m_objUseRelativeSpeed->SelectItem (buffer);

	orxConfig_PopSection ();
    }
}

void ObjectEditor::OnMouseClick (const string& widgetName)
{
    orxASSERT (m_object != orxNULL);

    if (widgetName == "ButtonChildList")
    {
	ListPopup* popup = orxCRAFT_CAST<ListPopup *>(
		CEDialogManager::GetInstance().MakeDialog(
		"ListPopup", string(m_object->GetModelName()) + ": Child List"));

	// Potential children are all objects
	//! @todo Are those only objects (ones with graphic defined) or also other entities??
	vector<std::string>& objects = OrxCraft::GetInstance().GetObjectList();
	vector<std::string> selection;

	m_object->PushConfigSection();
	orx_config_util::GetListIntoVector("ChildList", selection);
	m_object->PopConfigSection();

	popup->Fill(objects);
	popup->SetSelection(selection);
	PopupData* data = new PopupData;
	data->object = m_object;
	data->property = "ChildList";
	popup->SetUserData(data);
	popup->SetParent(this);
    }
    if (widgetName == "ButtonFXList")
    {
	ListPopup* popup = orxCRAFT_CAST<ListPopup *>(
		CEDialogManager::GetInstance().MakeDialog(
		"ListPopup", string(m_object->GetModelName()) + ": FX List"));

	// Potential children are all objects
	//! @todo Are those only objects (ones with graphic defined) or also other entities??
	vector<std::string>& objects = OrxCraft::GetInstance().GetFXList();
	vector<std::string> selection;

	m_object->PushConfigSection();
	orx_config_util::GetListIntoVector("FXList", selection);
	m_object->PopConfigSection();

	popup->Fill(objects);
	popup->SetSelection(selection);
	PopupData* data = new PopupData;
	data->object = m_object;
	data->property = "FXList";
	popup->SetUserData(data);
	popup->SetParent(this);
    }
}

void ObjectEditor::OnTextAccepted (const string& widgetName)
{
    orxASSERT (m_object != orxNULL);

    if (widgetName == "ObjectConfigName")
    {
	const string& name = m_objConfigName->GetSelectedItem ();
	//! @todo Better not to have this in the Scroll singleton
	ScrollObject *object =
	    OrxCraft::GetInstance ().GetObjectByName (name.c_str());
	SetObject (object);
    }

    // Push config section of edited object
    orxConfig_PushSection (m_object->GetModelName ());

    // Update config
    if (widgetName == "ObjAlpha")
    {
	const string& alpha = m_objAlpha->GetText ();
	orxConfig_SetString ("Alpha", alpha.c_str());
    }
    else if (widgetName == "ObjAngularVelocity")
    {
	const string& angularVelocity = m_objAngVelocity->GetText ();
	orxConfig_SetString ("AngularVelocity", angularVelocity.c_str());
    }
    else if (widgetName == "ObjAnimFreq")
    {
	orxFLOAT animFreq;
	orxString_ToFloat(m_objAnimFreq->GetText().c_str(),
		&animFreq, orxNULL);
	orxConfig_SetFloat ("AnimationFrequency", animFreq);
    }
    else if (widgetName == "ObjAnimSet")
    {
	orxASSERT (false);
    }
    else if (widgetName == "ObjAutoScroll")
    {
	orxASSERT (false);
	const string& autoScroll = m_objAutoScroll->GetSelectedItem ();
	orxConfig_SetString ("AutoScroll", autoScroll.c_str());
    }
    else if (widgetName == "ObjBlendMode")
    {
	const string& blendMode = m_objBlendMode->GetSelectedItem ();
	orxConfig_SetString ("BlendMode", blendMode.c_str());
    }
    else if (widgetName == "ObjBody")
    {
	orxASSERT (false);
    }
    else if (widgetName == "ObjClock")
    {
	orxASSERT (false);
    }
    else if (widgetName.find("ObjColor") != string::npos)
    {
	orxFLOAT newRFloat;
	orxFLOAT newGFloat;
	orxFLOAT newBFloat;
	orxString_ToFloat(m_objColorR->GetText().c_str(), &newRFloat, orxNULL);
	orxString_ToFloat(m_objColorG->GetText().c_str(), &newGFloat, orxNULL);
	orxString_ToFloat(m_objColorB->GetText().c_str(), &newBFloat, orxNULL);
	orxVECTOR newColor = { {newRFloat}, {newGFloat}, {newBFloat} };
	orxConfig_SetVector ("Color", &newColor);
    }
    else if (widgetName == "ObjDepthScale")
    {
	orxASSERT (false);
	const string& depthScale = m_objDepthScale->GetSelectedItem ();
	orxBOOL depthScaleBool;
	orxString_ToBool(depthScale.c_str(), &depthScaleBool, orxNULL);
	orxConfig_SetBool("DepthScale", depthScaleBool);
    }
    else if (widgetName == "ObjGraphic")
    {
	const string& graphic = m_objGraphic->GetSelectedItem ();
	orxConfig_SetString ("Graphic", graphic.c_str());
    }
    else if (widgetName == "ObjFlip")
    {
	const string& flip = m_objFlip->GetSelectedItem();
	orxConfig_SetString("Flip", flip.c_str());
    }
    else if (widgetName == "ObjChildList")
    {
	const string& children = m_objChildList->GetText();
	if(children.empty())
	    orxConfig_ClearValue("ChildList");
	else
	    orx_config_util::SetList("ChildList", children.c_str());
    }
    else if (widgetName == "ObjFXList")
    {
	orxASSERT (false);
    }
    else if (widgetName == "ObjLifeTime")
    {
	orxFLOAT lifeTime;
	orxString_ToFloat(m_objLifeTime->GetText().c_str(), &lifeTime, orxNULL);
	orxConfig_SetFloat ("LifeTime", lifeTime);
    }
    else if (widgetName == "ObjParentCam")
    {
	orxASSERT (false);
    }
    //! @todo check string::find syntax
    else if (widgetName.find("ObjPos") != string::npos)
    {
	orxFLOAT newXFloat;
	orxFLOAT newYFloat;
	orxFLOAT newZFloat;
	//! @todo use some std::string method??
	orxString_ToFloat(m_objPosX->GetText().c_str(), &newXFloat, orxNULL);
	orxString_ToFloat(m_objPosY->GetText().c_str(), &newYFloat, orxNULL);
	orxString_ToFloat(m_objPosZ->GetText().c_str(), &newZFloat, orxNULL);
	orxVECTOR newPosition = { {newXFloat}, {newYFloat}, {newZFloat} };
	orxConfig_SetVector ("Position", &newPosition);
    }
    else if (widgetName.find("ObjRepeat") != string::npos)
    {
	orxFLOAT XFloat;
	orxFLOAT YFloat;
	orxFLOAT ZFloat;
	orxString_ToFloat(m_objRepeatX->GetText().c_str(), &XFloat, orxNULL);
	orxString_ToFloat(m_objRepeatY->GetText().c_str(), &YFloat, orxNULL);
	orxString_ToFloat(m_objRepeatZ->GetText().c_str(), &ZFloat, orxNULL);
	orxVECTOR repeat = { {XFloat}, {YFloat}, {ZFloat} };
	orxConfig_SetVector ("Repeat", &repeat);
    }
    else if (widgetName == "ObjRotation")
    {
	const string& rotation = m_objRotation->GetText();
	orxConfig_SetString("Rotation", rotation.c_str());
    }
    else if (widgetName.find("ObjSpeed") != string::npos)
    {
	orxFLOAT newXFloat;
	orxFLOAT newYFloat;
	orxFLOAT newZFloat;
	orxString_ToFloat(m_objSpeedX->GetText().c_str(), &newXFloat, orxNULL);
	orxString_ToFloat(m_objSpeedY->GetText().c_str(), &newYFloat, orxNULL);
	orxString_ToFloat(m_objSpeedZ->GetText().c_str(), &newZFloat, orxNULL);
	orxVECTOR newSpeed = { {newXFloat}, {newYFloat}, {newZFloat} };
	orxConfig_SetVector ("Speed", &newSpeed);
    }
    else if (widgetName == "ObjScale")
    {
	orxFLOAT XFloat;
	orxFLOAT YFloat;
	orxFLOAT ZFloat;
	orxString_ToFloat(m_objScaleX->GetText().c_str(), &XFloat, orxNULL);
	orxString_ToFloat(m_objScaleY->GetText().c_str(), &YFloat, orxNULL);
	orxString_ToFloat(m_objScaleZ->GetText().c_str(), &ZFloat, orxNULL);
	orxVECTOR scale = { {XFloat}, {YFloat}, {ZFloat} };
	orxConfig_SetVector ("Scale", &scale);
    }
    else if (widgetName == "ObjShaderList")
    {
	orxASSERT (false);
    }
    else if (widgetName == "ObjSoundList")
    {
	orxASSERT (false);
    }
    else if (widgetName == "ObjSpawner")
    {
	orxASSERT (false);
    }

    orxConfig_PopSection ();

    // Update object in editor
    OrxCraft::GetInstance ().NeedObjectUpdate ();
}

void ObjectEditor::OnPopupFinish(const string& popupName,
	const string& popupTitle)
{
    ListPopup* popup = orxCRAFT_CAST<ListPopup *>(
	    CEDialogManager::GetInstance().GetDialog(popupName, popupTitle));
    const vector<string>& selection = popup->GetSelection();
    PopupData* data = static_cast<PopupData *>( popup->GetUserData() );

    data->object->PushConfigSection();
    if(selection.empty())
	orxConfig_ClearValue(data->property.c_str());
    else
    	//! @todo prepare SetList(string, vector<string>
	orx_config_util::SetList(data->property.c_str(),
		orxcraft_util::ListToString(selection).c_str());
    data->object->PopConfigSection();

    UpdateFields();
    // Update object in editor
    OrxCraft::GetInstance ().NeedObjectUpdate ();
}

void ObjectEditor::OnDestroy ()
{
    CEDialogManager::GetInstance().DestroyDialog(m_id);
    /*
     * Beyond this point the dialog was destroyed (delete was issued).
     * Make sure in is not accessed anymore.
     */
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
