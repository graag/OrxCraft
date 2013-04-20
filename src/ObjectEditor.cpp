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
#include "constants.h"

#include "DialogManager.h"
#include "ScrollCombobox.h"
#include "ScrollEditbox.h"
#include "ScrollPushButton.h"
#include "ListPopup.h"
#include "TreePopup.h"

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
    m_buttonChildList(NULL),
    m_buttonFXList(NULL)
{
}

void ObjectEditor::Init ()
{
    FindWidget("ObjAlpha")->ConfigBind(
	    "Alpha", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("ObjAngularVelocity")->ConfigBind(
	    "AngularVelocity", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("ObjPos0")->ConfigBind(
	    "Position", orxCRAFT_WIDGET_DATA_VECTOR0);
    FindWidget("ObjPos1")->ConfigBind(
	    "Position", orxCRAFT_WIDGET_DATA_VECTOR1);
    FindWidget("ObjPos2")->ConfigBind(
	    "Position", orxCRAFT_WIDGET_DATA_VECTOR2);
    FindWidget("ObjFXList")->ConfigBind(
    	    "FXList", orxCRAFT_WIDGET_DATA_LIST);
    FindWidget("ObjChildList")->ConfigBind(
    	    "ChildList", orxCRAFT_WIDGET_DATA_LIST);
    FindWidget("ObjAnimFreq")->ConfigBind(
	    "AnimationFrequency", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("ObjAnimSet")->ConfigBind("AnimationSet");
    FindWidget("ObjAutoScroll")->ConfigBind("AutoScroll");
    FindWidget("ObjBlendMode")->ConfigBind("BlendMode");
    FindWidget("ObjBody")->ConfigBind("Body");
    FindWidget("ObjChildJointList")->ConfigBind(
    	    "ChildJointList", orxCRAFT_WIDGET_DATA_LIST);
    FindWidget("ObjClock")->ConfigBind("Clock");
    FindWidget("ObjDepthScale")->ConfigBind(
	    "DepthScale", orxCRAFT_WIDGET_DATA_BOOL);
    FindWidget("ObjGraphic")->ConfigBind("Graphic");
    FindWidget("ObjFlip")->ConfigBind("Flip");
    FindWidget("ObjLifeTime")->ConfigBind(
	    "LifeTime", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("ObjParentCam")->ConfigBind("ParentCamera");
    FindWidget("ObjRepeat0")->ConfigBind(
	    "Repeat", orxCRAFT_WIDGET_DATA_VECTOR0);
    FindWidget("ObjRepeat1")->ConfigBind(
	    "Repeat", orxCRAFT_WIDGET_DATA_VECTOR1);
    FindWidget("ObjRepeat2")->ConfigBind(
	    "Repeat", orxCRAFT_WIDGET_DATA_VECTOR2);
    FindWidget("ObjRotation")->ConfigBind(
	    "Rotation", orxCRAFT_WIDGET_DATA_FLOAT);
    FindWidget("ObjScale0")->ConfigBind(
	    "Scale", orxCRAFT_WIDGET_DATA_VECTOR0);
    FindWidget("ObjScale1")->ConfigBind(
	    "Scale", orxCRAFT_WIDGET_DATA_VECTOR1);
    FindWidget("ObjScale2")->ConfigBind(
	    "Scale", orxCRAFT_WIDGET_DATA_VECTOR2);
    FindWidget("ObjSpeedX")->ConfigBind(
	    "Speed", orxCRAFT_WIDGET_DATA_VECTOR0);
    FindWidget("ObjSpeedY")->ConfigBind(
	    "Speed", orxCRAFT_WIDGET_DATA_VECTOR1);
    FindWidget("ObjSpeedZ")->ConfigBind(
	    "Speed", orxCRAFT_WIDGET_DATA_VECTOR2);
    FindWidget("ObjSmoothing")->ConfigBind(
	    "Smoothing", orxCRAFT_WIDGET_DATA_BOOL);
    FindWidget("ObjShaderList")->ConfigBind(
    	    "ShaderList", orxCRAFT_WIDGET_DATA_LIST);
    FindWidget("ObjSoundList")->ConfigBind(
    	    "SoundList", orxCRAFT_WIDGET_DATA_LIST);
    FindWidget("ObjSpawner")->ConfigBind("Spawner");
    FindWidget("ObjUseParentSpace")->ConfigBind(
	    "UseParentSpace", orxCRAFT_WIDGET_DATA_BOOL);
    FindWidget("ObjUseRelativeSpeed")->ConfigBind(
	    "UseRelativeSpeed", orxCRAFT_WIDGET_DATA_BOOL);
    FindWidget("ObjColor0")->ConfigBind(
	    "Color", orxCRAFT_WIDGET_DATA_VECTOR0);
    FindWidget("ObjColor1")->ConfigBind(
	    "Color", orxCRAFT_WIDGET_DATA_VECTOR1);
    FindWidget("ObjColor2")->ConfigBind(
	    "Color", orxCRAFT_WIDGET_DATA_VECTOR2);

    // Push buttons
    m_buttonChildList = (ScrollPushButton*)FindWidget("ButtonChildList");
    m_buttonFXList = (ScrollPushButton*)FindWidget("ButtonFXList");

    m_objConfigName = FindCombobox ("ObjectConfigName");
    orxASSERT(m_objConfigName != NULL);

    SetupFields ();
}

void ObjectEditor::SetupFields()
{
    vector<string> propList =
	OrxCraft::GetInstance().GetObjectList();

    m_objConfigName->Fill(propList);

    // Fill combo boxes
    orxConfig_PushSection("ObjectEditor");
    ScrollCombobox* box;

    propList.clear();
    box = FindCombobox("ObjGraphic");
    propList = OrxCraft::GetInstance().GetGraphicList();
    box->Fill(propList);

    propList.clear();
    box = FindCombobox("ObjAutoScroll");
    orxConf::GetListAsVector("AutoScroll", propList);
    box->Fill(propList);

    propList.clear();
    box = FindCombobox("ObjBlendMode");
    orxConf::GetListAsVector("BlendMode", propList);
    box->Fill(propList);

    propList.clear();
    box = FindCombobox("ObjFlip");
    orxConf::GetListAsVector("Flip", propList);
    box->Fill(propList);

    orxConfig_PopSection();

    ScrollObject *object =
	OrxCraft::GetInstance().GetObjectByName("O-Soldier");

    SetObject(object);
}

void ObjectEditor::SetObject(ScrollObject *object)
{
    m_object = object;
    UpdateFields();
}

ScrollObject* ObjectEditor::GetObject()
{
    return m_object;
}

void ObjectEditor::UpdateFields() const
{
    if (m_object != orxNULL)
    {
	string obj_name = m_object->GetModelName();

	// Config name
	m_objConfigName->SetSelection(obj_name);

	orxConfig_PushSection(obj_name.c_str());

	vector<ScrollWidget *>::const_iterator it;
	string widget_name;
	for(it = m_widgetList.begin(); it != m_widgetList.end(); it++) {
	    widget_name = (*it)->GetName();
	    if(widget_name == "ObjectConfigName")
		continue;
	    if(widget_name == "ButtonChildList")
		continue;
	    if(widget_name == "ButtonFXList")
		continue;
	    (*it)->ConfigRead();
	}

	orxConfig_PopSection ();
    }
}

void ObjectEditor::OnMouseClick (const string& widgetName)
{
    orxASSERT (m_object != orxNULL);

    if (widgetName == "ButtonChildList")
    {
/*
	ListPopup* popup = orxCRAFT_CAST<ListPopup *>(
		CEDialogManager::GetInstance().MakeDialog(
		"ListPopup", string(m_object->GetModelName()) + ": Child List"));
	orxASSERT(popup != orxNULL);

	// Potential children are all objects
	//! @todo Are those only objects (ones with graphic defined) or also other entities??
	vector<std::string> objects =
	    OrxCraft::GetInstance().GetObjectListSafe(
		    m_object->GetModelName()
		    );
	vector<std::string> selection;

	m_object->PushConfigSection();
	orxConf::GetListAsVector("ChildList", selection);
	m_object->PopConfigSection();

	popup->Fill(objects);
	popup->SetSelection(selection);
	PopupData* data = new PopupData;
	data->object = m_object;
	data->property = "ChildList";
	popup->SetUserData(data);
	popup->SetParent(this);
*/
	TreePopup* popup = orxCRAFT_CAST<TreePopup *>(
		OrxCraft::GetInstance().GetDialogManager()->MakeDialog(
		"TreePopup", string(m_object->GetModelName()) + ": Child List"));
	orxASSERT(popup != orxNULL);

	// Potential children are all objects
	//! @todo Are those only objects (ones with graphic defined) or also other entities??
	vector<std::string> objects =
	    OrxCraft::GetInstance().GetObjectListSafe(
		    m_object->GetModelName()
		    );
	vector<std::string> selection;
	vector<ScrollTreePair> objects_group;

	vector<string>::iterator it;
	for(it = objects.begin(); it != objects.end(); it++) {
	    string setName = "";
	    ScrollTreePair item;

	    orxConfig_PushSection(it->c_str());
	    // Has ScrollEd type and a graphic?
	    if(orxConfig_HasValue(scrollEdSectionName)) {
	    	setName = orxConfig_GetString(scrollEdSectionName);
	    }
	    orxConfig_PopSection();

	    item.first = setName;
	    item.second = *it;
	    objects_group.push_back(item);
	}

	m_object->PushConfigSection();
	orxConf::GetListAsVector("ChildList", selection);
	m_object->PopConfigSection();

	popup->Fill(objects_group);
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
		OrxCraft::GetInstance().GetDialogManager()->MakeDialog(
		"ListPopup", string(m_object->GetModelName()) + ": FX List"));
	orxASSERT(popup != orxNULL);

	// Potential children are all objects
	//! @todo Are those only objects (ones with graphic defined) or also other entities??
	vector<std::string>& objects = OrxCraft::GetInstance().GetFXList();
	vector<std::string> selection;

	m_object->PushConfigSection();
	orxConf::GetListAsVector("FXList", selection);
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

void ObjectEditor::OnTextAccepted(const string& widgetName)
{
    orxASSERT(m_object != orxNULL);

    if (widgetName == "ObjectConfigName")
    {
	const string& name = m_objConfigName->GetSelection();
	//! @todo Better not to have this in the Scroll singleton
	ScrollObject *object =
	    OrxCraft::GetInstance().GetObjectByName(name);
	SetObject(object);
    } else {

	// Push config section of edited object
	orxConfig_PushSection(m_object->GetModelName());

	// Update Orx config in memory from value entered in control
	ScrollWidget* box = FindWidget(widgetName);
	orxASSERT(box != orxNULL);
	box->ConfigUpdate();

	orxConfig_PopSection ();
    }

    // Update object in editor
    OrxCraft::GetInstance().NeedObjectUpdate();
}

void ObjectEditor::OnPopupFinish(const string& popupName,
	const string& popupTitle)
{
    if(popupName == "ListPopup") {
	ListPopup* popup = orxCRAFT_CAST<ListPopup *>(
		OrxCraft::GetInstance().GetDialogManager()->GetDialog(
		    popupName, popupTitle));
	orxASSERT(popup != orxNULL);
	const vector<string>& selection = popup->GetSelection();
	PopupData* data = static_cast<PopupData *>( popup->GetUserData() );

	data->object->PushConfigSection();
	if(selection.empty())
	    orxConfig_ClearValue(data->property.c_str());
	else
	    //! @todo prepare SetList(string, vector<string>
	    orxConf::SetList(data->property.c_str(),
		    orxUtil::ListToString(selection).c_str());
	data->object->PopConfigSection();
    } else if (popupName == "TreePopup") {
	TreePopup* popup = orxCRAFT_CAST<TreePopup *>(
		OrxCraft::GetInstance().GetDialogManager()->GetDialog(
		    popupName, popupTitle));
	orxASSERT(popup != orxNULL);
	const vector<string>& selection = popup->GetSelection();
	PopupData* data = static_cast<PopupData *>( popup->GetUserData() );

	data->object->PushConfigSection();
	if(selection.empty())
	    orxConfig_ClearValue(data->property.c_str());
	else
	    //! @todo prepare SetList(string, vector<string>
	    orxConf::SetList(data->property.c_str(),
		    orxUtil::ListToString(selection).c_str());
	data->object->PopConfigSection();
    }

    UpdateFields();
    // Update object in editor
    OrxCraft::GetInstance().NeedObjectUpdate();
}

void ObjectEditor::OnDestroy ()
{
    OrxCraft::GetInstance().GetDialogManager()->DestroyDialog(m_id);
    /*
     * Beyond this point the dialog was destroyed (delete was issued).
     * Make sure in is not accessed anymore.
     */
}

void ObjectEditor::OnReset ()
{
    const string& name = m_objConfigName->GetSelection();
    //! @todo Better not to have this in the Scroll singleton
    ScrollObject *object =
	OrxCraft::GetInstance().GetObjectByName(name);
    SetObject(object);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
