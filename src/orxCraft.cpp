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
 * @file OrxCraft.cpp
 * @date 2012-05-04
 * @author fritz@fritzmahnke.com
 *
 */
#define __SCROLL_IMPL__
#include "orxCraft.h"
#undef  __SCROLL_IMPL__

#include "InfoWindow.h"
#include "ObjectEditor.h"
#include "FXSlotEditorWindow.h"

#include "cegui/CEGUIGui.h"
#include "cegui/CEGUIDialogManager.h"

#include "constants.h"
#include "orxcraft_util.h"

#include "mkpath/mkpath.h"

#include <string>
#include <set>

using std::string;
using std::set;

// Widgets
static const orxSTRING infoWindow = "O-InfoWindow";

string OrxCraft::m_projectFileName;
string OrxCraft::m_projectPath;
orxFLOAT OrxCraft::m_localTime;
orxFLOAT OrxCraft::m_newKeyTimeStamp;
orxU32 OrxCraft::m_keyRepeatCounter;

OrxCraft::OrxCraft () :
    m_gui               (NULL),
    m_dialogManager     (NULL),
    m_dirty             (false),
    m_dirtySave         (false),
    m_dirtyAutosave     (false),
    m_autoSaveTimeStamp (0),
    m_autoSaveInterval  (0)
{
    m_localTime = 0;
    m_newKeyTimeStamp = 0;
}

ScrollObject * OrxCraft::GetObjectByName (const string& name) const
{
    ScrollObject *foundObject = NULL;

    for (ScrollObject *obj = GetNextObject (orxNULL);
	 obj != orxNULL;
	 obj = GetNextObject (obj))
    {
	// We are only interested in editable objects (directly loaded by
	// OrxCraft) not in auto created children objects. Filter children out
	// using FlagSave.
	if(obj->TestFlags(ScrollObject::FlagSave) &&
		name == obj->GetModelName())
	{
	    foundObject = obj;
	    break;
	}
    }
    return foundObject;
}

vector<string> OrxCraft::GetObjectListSafe(const string& name)
{
    vector<string>::iterator it;
    vector<string> result;
    ScrollObject* obj = GetObjectByName(name);

    for(it=m_objectList.begin(); it != m_objectList.end(); it++) {
	// Cannot be child of self
	if(*it == name)
	    continue;
	ScrollObject* parent = GetObjectByName(*it);
	// Check if obj is not child of "it" to avoid cycles
	if(!orxUtil::IsChild(obj, parent)) {
	    result.push_back(*it);
	}
    }
    return result;
}

orxSTATUS OrxCraft::Init ()
{
    orxSTATUS eResult = orxSTATUS_SUCCESS;

    orxPARAM  stParams;
    // Asks for command line project parameter
    orxMemory_Zero (&stParams, sizeof(orxPARAM));
    stParams.pfnParser  = OrxCraft::ProcessParams;
    stParams.u32Flags   = orxPARAM_KU32_FLAG_NONE;
    stParams.zShortName = projectParamShortName;
    stParams.zLongName  = projectParamLongName;
    stParams.zShortDesc = projectParamShortDesc;
    stParams.zLongDesc  = projectParamLongDesc;
    orxParam_Register (&stParams);

    // Load things we want to edit from config
    InitConfig ();
    SetupConfig ();
    LoadUserSettings ();

    // Create instance of dialog manager
    m_dialogManager = OrxCraftFactory::CreateDialogManager();

    // Init GUI system
    m_gui = OrxCraftFactory::CreateGui();
    m_gui->Init();

    CreateObject (infoWindow);

    // Init object editor
    m_dialogManager->OpenDialog ("ObjectEditor");
    // Init FX slot editor
    m_dialogManager->OpenDialog ("FXSlotEditor");
    // Init project browser window
    m_dialogManager->OpenDialog ("ProjectBrowser");

    orxEvent_AddHandler (orxEVENT_TYPE_INPUT, EventHandler);

    return eResult;
}

orxSTATUS OrxCraft::Run ()
{
  orxSTATUS eResult = orxSTATUS_SUCCESS;

  // Quitting?
  if(orxInput_IsActive (inputQuit))
  {
    // Updates result
    eResult = orxSTATUS_FAILURE;
  }

  // Done!
  return eResult;
}

void OrxCraft::Exit ()
{
    delete m_dialogManager;
    m_dialogManager = NULL;
    delete m_gui;
    m_gui = NULL;
}

void OrxCraft::BindObjects ()
{
/*
    orxConfig_PushSection("Display");
    string toolkit = orxConfig_GetString("Toolkit");
    orxConfig_PopSection();
    if(toolkit == "CEGUI") {
	ScrollBindObject<CEGUIGui::CEGUIScrollObject> (scrollGUIName);
    } else {
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		"Unknown widget toolkit selected: %s", toolkit.c_str());
	orxASSERT(false);
    }
*/

    OrxCraftFactory::BindObjects();
}

void OrxCraft::Update (const orxCLOCK_INFO &_rstInfo)
{
    orxSTATUS eResult;

    // Want to update the objects' state?
    if (m_dirty)
    {
	m_dirty = false;
	// Do setup again
	SetupConfig ();
    }

    // Updates local time
    m_localTime += _rstInfo.fDT;

    // Uses autosave?
    if (m_autoSaveInterval > orxFLOAT_0)
    {
	// Is it time?
	if (m_localTime >= m_autoSaveTimeStamp + m_autoSaveInterval)
	{
	    // Are there changes to save?
	    if (m_dirtyAutosave)
	    {
		// Saves backup
		eResult = SaveProject(true);
		// Successful?
		if (eResult != orxSTATUS_FAILURE)
		{
		    // Adds action display
		    AddActionDisplay (uiStringAutoSave);
		}
	    }

	    /*
	     * Updates time stamp
	     * Do it even if there were no changes otherwise autosave will be
	     * triggered immediatly after a change was made instead in fixed
	     * intervals.
	     */
	    m_autoSaveTimeStamp = m_localTime;
	}
    }

    // Save?
    if (orxInput_IsActive (inputSave) && orxInput_HasNewStatus (inputSave) &&
    	m_dirtySave)
    {
	// Save project
	eResult = SaveProject ();
	// Successful?
	if (eResult != orxSTATUS_FAILURE)
	{
	    AddActionDisplay (uiStringSave);
	}
    }

    m_gui->Update(_rstInfo);

    const orxSTRING kbdInput= orxKeyboard_ReadString();
    if(orxString_GetLength(kbdInput) != 0)
    {
	m_gui->InputString(kbdInput);
    }

    KeyRepeat();
}

void OrxCraft::NeedObjectUpdate ()
{
    m_dirty = true;
    m_dirtySave = true;
    m_dirtyAutosave = true;
}

void OrxCraft::InitConfig ()
{
    orxConfig_Load (m_projectFileName.c_str());
}

void OrxCraft::SetupConfig ()
{
    // For all current objects
    for(ScrollObject *obj = GetNextObject(), *nextObj = GetNextObject(obj);
        obj;
        obj = nextObj, nextObj = GetNextObject(nextObj))
    {
	// Do not delete OrxCraft internal objects
	obj->PushConfigSection();
	if (orxConfig_GetBool ("OrxCraftSection") == orxTRUE) {
	    obj->PopConfigSection();
	    continue;
	}
	obj->PopConfigSection();
	// Only delete editable objects (directly loaded by OrxCraft). This
	// will make sure we do not interfere with orx/Scroll auto destruction
	// of child objects.
	if(obj->TestFlags(ScrollObject::FlagSave))
	{
	    // Deletes it
	    DeleteObject(obj);
	}
    }

    m_groupList.clear();
    m_objectList.clear();
    m_graphicList.clear();
    m_fxList.clear();
    m_fxSlotList.clear();

    for (int i = 0, sectionCounter = orxConfig_GetSectionCounter ();
	 i < sectionCounter;
	 i++)
    {
	const orxSTRING sectionName = orxConfig_GetSection (i);
	orxConfig_PushSection (sectionName);
	if (orxConfig_GetBool ("OrxCraftSection") != orxTRUE)
	{
	    const orxSTRING graphic = orxConfig_GetString ("Graphic");
	    // Does it have a Graphic property?
	    // @todo What about composite objects as in tutorials Frame and
	    //      Physics? ChildList property? But then how to build object
	    //      hieratchy?
	    if (orxString_Compare (graphic, "") != 0)
	    {
		// It's an object
		m_objectList.push_back (sectionName);
		ScrollObject* newObj = CreateObject (sectionName);
		// Mark it with FlagSave. FlagSave is used by ScrollEd to mark
		// objects that should be written explicetely to map. We will
		// use it to mark editable objects (the ones loaded by OrxCraft
		// directly.) This will allow to handle properly cases with
		// children. When an object with ChildList is created its
		// children are created by orx on the fly and will not have
		// FlagSave set. We do not want to destroy them by hand, as
		// they are auto destroyed by orx (we would end up with double
		// free).
		newObj->SetFlags(ScrollObject::FlagSave);
		const orxSTRING scrollEdSet = orxConfig_GetString(scrollEdSectionName);
		if(orxString_GetLength(scrollEdSet))
		    m_groupList.insert(scrollEdSet);
		orxConfig_PopSection ();
		continue;
	    }
	    // Does it have a Texture property?
	    const orxSTRING texture = orxConfig_GetString ("Texture");
	    if (orxString_Compare (texture, "") != 0)
	    {
		// It's a graphic
		m_graphicList.push_back (sectionName);
		//! @todo DO we really want to create graphics ???
		//! @todo If yes than delete them first as for objects
		// ScrollObject* newObj = CreateObject (sectionName); 
		// newObj->SetFlags(ScrollObject::FlagSave); 
		orxConfig_PopSection ();
		continue;
	    }
	    // Does it have a SlotList property?
	    const orxSTRING slotList = orxConfig_GetString ("SlotList");
	    if (orxString_Compare (slotList, "") != 0)
	    {
		// It's a graphic
		m_fxList.push_back (sectionName);
		orxConfig_PopSection ();
		continue;
	    }
	    // Does it have a Curve property?
	    const orxSTRING curve = orxConfig_GetString ("Curve");
	    if (orxString_Compare (curve, "") != 0)
	    {
		// It's an FXSlot
		m_fxSlotList.push_back (sectionName);
		orxConfig_PopSection ();
		continue;
	    }
	}
	orxConfig_PopSection ();
    }

    if(m_dialogManager != NULL)
	m_dialogManager->OnReset();
}

void OrxCraft::LoadUserSettings ()
{
    // Get UserSettings section
    orxConfig_PushSection (userSettingsSection);
    // AutoSave interval
    m_autoSaveInterval = orxConfig_GetFloat ("AutoSaveInterval");
    m_autoSaveTimeStamp = m_autoSaveInterval;
    // Cleanup
    orxConfig_PopSection ();
}

orxSTATUS OrxCraft::SaveEditorConfig () const
{
    orxSTATUS eResult;
    eResult = orxConfig_Save (userSettingsFile, false, &SaveConfigFilter);
    return eResult;
}

orxSTATUS OrxCraft::SaveProject (bool backup)
{
    orxSTATUS eResult;
    string fileName;
    string filePath;
    orxFILE_INFO fileInfo;
    orxFILE* file;

    // Main project file
    fileName = m_projectFileName;
    if(backup)
	fileName += ".swp";
    eResult = orxConfig_Save (fileName.c_str(), false, &SaveProjectFilter);
    if(eResult == orxSTATUS_FAILURE)
	return eResult;

    // Open main project file in append mode
    orxFile_GetInfo(fileName.c_str(), &fileInfo);
    file = orxFile_Open(fileInfo.zFullName, orxFILE_KU32_FLAG_OPEN_APPEND);
    if(file == orxNULL)
	return orxSTATUS_FAILURE;

    // Section files
    set<string>::iterator it = m_groupList.begin();
    for( ; it != m_groupList.end(); it++)
    {
	// Filename: composite of project path, section name and "ini" extension
	fileName = m_projectPath + "/" + *it + ".ini";
	if(backup)
	    fileName += ".swp";

	// Make sure that of file we are going to write to exists.
	// If not create it along with all parents ...
	size_t pos = fileName.rfind("/", fileName.size());
	filePath.assign(fileName, 0, pos);
	orxFILE_INFO fileInfo;
	orxFile_GetInfo(filePath.c_str(), &fileInfo);
	if((fileInfo.u32Flags & orxFILE_KU32_FLAG_INFO_DIR) == 0)
	{
	    // Path does not exist or is not a directory
	    // Lets try to create it
	    int result = mkpath(filePath.c_str(), S_IRWXU);
	    if(result != 0)
		return orxSTATUS_FAILURE;
	}

	// Write data to the file
	eResult = orxConfig_Save (fileName.c_str(), false, &SaveProjectFilter);
	if(eResult == orxSTATUS_FAILURE)
	    return eResult;

	orxFile_Print(file, "@%s/%s.ini@\n",
		m_projectPath.c_str(),
		it->c_str());
    }

    orxFile_Close(file);

    // Successful?
    if (eResult != orxSTATUS_FAILURE)
    {
	if(backup)
	{
	    m_dirtyAutosave = false;
	    // Adds action display
	    AddActionDisplay(uiStringAutoSave);
	}
	else
	{
	    m_dirtySave = false;
	    // Adds action display
	    AddActionDisplay(uiStringSave);
	}
    }

    return eResult;
}

void OrxCraft::OnMouseDown ()
{
    m_gui->InputMouseDown ();
}

void OrxCraft::OnMouseUp ()
{
    m_gui->InputMouseUp ();
}

void OrxCraft::OnKeyPress (const orxSTRING key)
{
    m_gui->InputKeyPress (key);
}

void OrxCraft::OnKeyRelease (const orxSTRING key)
{
    m_gui->InputKeyRelease (key);
}

void OrxCraft::KeyRepeat()
{
    orxFLOAT delay = inputRepeatDelay;
    // After 2 repeats we can speedup the repeater
    if(m_keyRepeatCounter > 1)
    {
	delay = inputRepeatDelayFast;
	// Just to make sure that the counter will not overflow ;-)
	m_keyRepeatCounter = 10;
    }

    // Backspace
    if (orxInput_IsActive (inputBackspace) &&
	    !orxInput_HasNewStatus (inputBackspace) &&
	    m_localTime > m_newKeyTimeStamp + delay)
    {
	OrxCraft::GetInstance().OnKeyPress(inputBackspace);
	m_newKeyTimeStamp = m_localTime;
	m_keyRepeatCounter++;
    }
    // Delete
    if (orxInput_IsActive (inputDelete) &&
	    !orxInput_HasNewStatus (inputDelete) &&
	    m_localTime > m_newKeyTimeStamp + delay)
    {
	OrxCraft::GetInstance().OnKeyPress(inputDelete);
	m_newKeyTimeStamp = m_localTime;
	m_keyRepeatCounter++;
    }
    // LeftArrow
    if (orxInput_IsActive (inputLeftArrow) &&
	    !orxInput_HasNewStatus (inputLeftArrow) &&
	    m_localTime > m_newKeyTimeStamp + delay)
    {
	OrxCraft::GetInstance().OnKeyPress(inputLeftArrow);
	m_newKeyTimeStamp = m_localTime;
	m_keyRepeatCounter++;
    }
    // RightArrow
    if (orxInput_IsActive (inputRightArrow) &&
	    !orxInput_HasNewStatus (inputRightArrow) &&
	    m_localTime > m_newKeyTimeStamp + delay)
    {
	OrxCraft::GetInstance().OnKeyPress(inputRightArrow);
	m_newKeyTimeStamp = m_localTime;
	m_keyRepeatCounter++;
    }
    // UpArrow
    if (orxInput_IsActive (inputUpArrow) &&
	    !orxInput_HasNewStatus (inputUpArrow) &&
	    m_localTime > m_newKeyTimeStamp + delay)
    {
	OrxCraft::GetInstance().OnKeyPress(inputUpArrow);
	m_newKeyTimeStamp = m_localTime;
	m_keyRepeatCounter++;
    }
    // DownArrow
    if (orxInput_IsActive (inputDownArrow) &&
	    !orxInput_HasNewStatus (inputDownArrow) &&
	    m_localTime > m_newKeyTimeStamp + delay)
    {
	OrxCraft::GetInstance().OnKeyPress(inputDownArrow);
	m_newKeyTimeStamp = m_localTime;
	m_keyRepeatCounter++;
    }
}

orxSTATUS orxFASTCALL OrxCraft::EventHandler (const orxEVENT *_pstEvent)
{
    orxSTATUS result = orxSTATUS_SUCCESS;

    // Key pressed?
    if (_pstEvent->eType == orxEVENT_TYPE_INPUT)
    {
	if(_pstEvent->eID == orxINPUT_EVENT_ON)
	{
	    orxINPUT_EVENT_PAYLOAD *pstPayload;
	    pstPayload = reinterpret_cast<orxINPUT_EVENT_PAYLOAD *> (
		_pstEvent->pstPayload);
	    switch (pstPayload->aeType[0])
	    {
	    case orxINPUT_TYPE_KEYBOARD_KEY:
		OrxCraft::GetInstance().OnKeyPress(pstPayload->zInputName);
		m_newKeyTimeStamp = m_localTime;
		m_keyRepeatCounter = 0;
		break;
	    case orxINPUT_TYPE_MOUSE_BUTTON:
		if (orxString_Compare (inputLeftMB, pstPayload->zInputName) == 0)
		    OrxCraft::GetInstance ().OnMouseDown ();
		break;
	    default:
	    	break;
	    }
	}
	if(_pstEvent->eID == orxINPUT_EVENT_OFF)
	{
	    orxINPUT_EVENT_PAYLOAD *pstPayload;
	    pstPayload = reinterpret_cast<orxINPUT_EVENT_PAYLOAD *> (
		_pstEvent->pstPayload);
	    switch (pstPayload->aeType[0])
	    {
	    case orxINPUT_TYPE_KEYBOARD_KEY:
		OrxCraft::GetInstance ().OnKeyRelease (pstPayload->zInputName);
		break;
	    case orxINPUT_TYPE_MOUSE_BUTTON:
		if (orxString_Compare (inputLeftMB, pstPayload->zInputName) == 0)
		    OrxCraft::GetInstance ().OnMouseUp ();
		break;
	    default:
	    	break;
	    }
	}
    }

    return result;
}

orxBOOL orxFASTCALL OrxCraft::SaveConfigFilter (const orxSTRING sectionName,
					        const orxSTRING keyName,
						const orxSTRING fileName,
						orxBOOL useEncryption)
{
    orxBOOL saveIt = orxFALSE;

    // Save only UserSettings section
    if (orxString_Compare (sectionName, userSettingsSection) == 0)
    {
	saveIt = orxTRUE;
    }

    return saveIt;
}

orxBOOL orxFASTCALL OrxCraft::SaveProjectFilter (const orxSTRING sectionName,
						 const orxSTRING keyName,
						 const orxSTRING fileName,
						 orxBOOL useEncryption)
{
    orxBOOL saveIt = orxFALSE;

    /*
     *  All objects that do not have OrxCraftSection key are part of the project
     *  data and will be saved.
     */
    orxConfig_PushSection (sectionName);
    orxBOOL isOrxCraftSection = orxConfig_GetBool (orxCraftSectionName);
    string scrollEdSection = orxConfig_GetString(scrollEdSectionName);
    orxConfig_PopSection ();

    // NOT one of our editor's sections?
    if (! isOrxCraftSection)
    {
	// Filter out Scroll run time objects
	if(orxString_GetLength(sectionName) == 6)
	    if(sectionName[0] == 'R' && sectionName[1] == 'T' &&
		    sectionName[2] >= '0' && sectionName[2] <= '9' &&
		    sectionName[3] >= '0' && sectionName[3] <= '9' &&
		    sectionName[4] >= '0' && sectionName[4] <= '9' &&
		    sectionName[5] >= '0' && sectionName[5] <= '9'
	      )
		return orxFALSE;

	// Objects with "ScrollEdSet" property are stored in separate files
	if(!scrollEdSection.empty())
	{
	    size_t pos = string(fileName).rfind(".ini",
		    orxString_GetLength(fileName));
	    string fileNameStripped;
	    fileNameStripped.assign(fileName, m_projectPath.size() + 1,
		    pos - (m_projectPath.size() + 1));
	    if(scrollEdSection == fileNameStripped)
		saveIt = orxTRUE;
	}
	else
	// Other objects are stored in main project file
	{
	    if(orxString_NCompare(
			m_projectFileName.c_str(),
			fileName,
			m_projectFileName.size()) == 0
	      )
		saveIt = orxTRUE;
	}
    }

    return saveIt;
}

orxSTATUS orxFASTCALL OrxCraft::ProcessParams (orxU32 paramCount,
					       const orxSTRING params[])
{
    orxSTATUS eResult;

    // Has a valid project parameter?
    if (paramCount >= 2 &&
     (orxString_Compare (params[0], projectParamLong) == 0 ||
      orxString_Compare (params[0], projectParamShort) == 0))
    {
	// Stores projects file name
	m_projectFileName = params[1];
	// Store project path
	string::size_type pos = m_projectFileName.rfind(
		"/", m_projectFileName.length());
	if (pos != string::npos) {
	    m_projectPath.assign(m_projectFileName, 0, pos);
	}

	// Updates result
	eResult = orxSTATUS_SUCCESS;
    }
    else
    {
	// Updates result
	eResult = orxSTATUS_FAILURE;
    }

    // Done!
    return eResult;
}

orxSTATUS OrxCraft::AddActionDisplay (const orxSTRING action) const
{
    orxSTATUS eResult = orxSTATUS_FAILURE;

    // Has valid action
    if (action)
    {
	orxOBJECT *pstActionText;

	// Creates action test
	pstActionText = orxObject_CreateFromConfig (uiActionText);

	// Valid?
	if (pstActionText)
	{
	  // Updates its content
	  orxObject_SetTextString (pstActionText, action);

	  // Updates result
	  eResult = orxSTATUS_SUCCESS;
	}
    }

    // Done!
    return eResult;
}

void OrxCraft::OrxCraftFactory::BindObjects()
{
    orxConfig_PushSection("Display");
    string toolkit = orxConfig_GetString("Toolkit");
    orxConfig_PopSection();
    if(toolkit == "CEGUI") {
	ScrollBindObject<CEGUIGui::CEGUIScrollObject> (scrollGUIName);
    } else {
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		"Unknown widget toolkit selected: %s", toolkit.c_str());
	orxASSERT(false);
    }
}

ScrollGui* OrxCraft::OrxCraftFactory::CreateGui()
{
    orxConfig_PushSection("Display");
    string toolkit = orxConfig_GetString("Toolkit");
    orxConfig_PopSection();
    if(toolkit == "CEGUI") {
	return new CEGUIGui();
    } else {
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		"Unknown widget toolkit selected: %s", toolkit.c_str());
	orxASSERT(false);
    }

    return orxNULL;
}

DialogManager* OrxCraft::OrxCraftFactory::CreateDialogManager()
{
    orxConfig_PushSection("Display");
    string toolkit = orxConfig_GetString("Toolkit");
    orxConfig_PopSection();
    if(toolkit == "CEGUI") {
	return new CEGUIDialogManager();
    } else {
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		"Unknown widget toolkit selected: %s", toolkit.c_str());
	orxASSERT(false);
    }

    return orxNULL;
}

int main (int argc, char **argv)
{
    // Executes game
    OrxCraft::GetInstance ().Execute (argc, argv);

    // Done!
    return EXIT_SUCCESS;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
