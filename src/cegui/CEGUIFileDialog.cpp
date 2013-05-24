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
 * @file CEGUIFileDialog.cpp
 * @date 2013-04-19
 * @author graag@o2.pl
 *
 * CEGUI File picker implementation.
 * Based on code written by spookyboo ported to Linux by ancestral:
 * http://www.ogre3d.org/tikiwiki/tiki-index.php?page=ceguicommonfiledialog
 */

#include "cegui/CEGUIFileDialog.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "orx.h"
#include "orxCraft.h"
#include "DialogManager.h"
#include "AlertPopup.h"
#include "ConfirmPopup.h"

#include "ScrollCombobox.h"
#include "ScrollListbox.h"
#include "ScrollEditbox.h"

#include "CEGUI.h"

using std::string;
using std::vector;
using std::sort;
using std::find;
using std::map;

using CEGUI::WindowManager;
using CEGUI::FrameWindow;
using CEGUI::PushButton;
using CEGUI::Combobox;
using CEGUI::Listbox;
using CEGUI::ListboxTextItem;
using CEGUI::Event;

const string NAME_LAYOUT_COMMON_FILE_DIALOG        = "FileDialog.layout";
const string NAME_FRAMEWINDOW_COMMON_FILE_DIALOG    = "FileDialog";
const string NAME_PUSHBUTTON_BACK            = "FileDialog/BackButton";
const string NAME_PUSHBUTTON_OK                = "FileDialog/OkButton";
const string NAME_PUSHBUTTON_CANCEL            = "FileDialog/CancelButton";
const string NAME_EDITBOX_FILE                = "FileDialog/FileEditbox";
const string NAME_EDITBOX_PATH                = "FileDialog/PathEditbox";
const string NAME_COMBOBOX_EXTENSION            = "FileDialog/ExtensionCombobox";
const string NAME_COMBOBOX_DRIVE            = "FileDialog/DriveCombobox";
const string NAME_LISTBOX_FILES                = "FileDialog/Listbox";

const string NAME_PROMPT_LOAD        = "AlertPopup";
const string NAME_PROMPT_SAVE        = "ConfirmPopup";

const string DEFAULT_TEXT_DIALOG_LOAD            = "Load";
const string DEFAULT_TEXT_DIALOG_SAVE            = "Save As";
const string DEFAULT_TEXT_PROMPT_LOAD            = "Cannot find the requested file.";
const string DEFAULT_TEXT_PROMPT_SAVE            = "The file already exist. Replace it?";

const string BRACKET_OPEN                = "<";
const string BRACKET_CLOSED              = ">";
const char   BRACKET_OPEN_ASCII          = 0x03C;

#ifdef __orxWINDOWS__

    #include <direct.h>
    #include <io.h>

    // Path separator
    const string SEPARATOR      = "\\";
    // global_file_tree - indicates that the file system hierarchy has single
    // root and "drives" are actually bookmarks. When adjusting path current
    // drive will be changed.
    const bool global_file_tree = false;

    #define CEGUI_MAX_PATH _MAX_PATH

#else /* __orxWINDOWS__ */

    #include <unistd.h>
    #include <stdlib.h>

    // Path separator
    const string SEPARATOR      = "/";
    // global_file_tree - indicates that the file system hierarchy has single
    // root and "drives" are actually bookmarks. When adjusting path current
    // drive will be changed.
    const bool global_file_tree = true;

    #define CEGUI_MAX_PATH PATH_MAX

#endif /* __orxWINDOWS__ */

CEGUIFileDialog::CEGUIFileDialog(const string& name, const string& title):
    FileDialog(name, title)
{
}

void CEGUIFileDialog::Init()
{
    // Find the CEGUI window created by DialogManager
    orxASSERT(
	    CEGUI::WindowManager::getSingleton().isWindowPresent(
		m_windowName)
	    );
    _mWindow = CEGUI::WindowManager::getSingleton().getWindow(
	    m_windowName);

    // Set title
    if(! m_title.empty())
	_mWindow->setText(m_title);
    else
	_mWindow->setText(m_name);

    // Subscribe to close window event
    _mWindow->subscribeEvent (FrameWindow::EventCloseClicked,
	    Event::Subscriber (&CEGUIFileDialog::_handleClose, this));

    // Find the widgets
    _mEdtFile = FindEditbox(NAME_EDITBOX_FILE);
    _mEdtPath = FindEditbox(NAME_EDITBOX_PATH);
    _mCmbDrives = FindCombobox(NAME_COMBOBOX_DRIVE);
    _mCmbExtensions = FindCombobox(NAME_COMBOBOX_EXTENSION);
    _mLbxFiles = FindListbox(NAME_LISTBOX_FILES);

    // Populate the drive combobox
    _determineDrives ();
    _mCmbDrives->Fill(_drives);
    _mCmbDrives->SetSelection(_getCurrentDrive());

    _mEdtPath->SetText(_getPathOfSelectedDrive()->szAbsolutePath);
}

/*************************************************************************
  CCEGUIFileDialog::~CCEGUIFileDialog
  Delete the _Path objects. Use the vector, because iterating through
  the hash_map gives problems
 *************************************************************************/
CEGUIFileDialog::~CEGUIFileDialog (void)
{
    map<string, _Path*>::iterator pos;
    for (pos = _currentPaths.begin(); pos != _currentPaths.end(); pos++)
    {
	if (pos->second)
	    delete pos->second;
    }

    _currentPaths.clear();
}

/*************************************************************************
  CCEGUIFileDialog::_closeWindow
  Actually closes the window
 *************************************************************************/
void CEGUIFileDialog::_closeWindow (void)
{
    // Notify others that we are closing
    SignalClose(m_name, m_id);
    // Ask the DialogManager to close us
    OrxCraft::GetInstance().GetDialogManager()->CloseDialog(m_id);
}

/*************************************************************************
  CCEGUIFileDialog::_openPromptWindowLoad
  Opens the prompt window in case of loading
 *************************************************************************/
void CEGUIFileDialog::_openPromptWindowLoad (void)
{
    // Open popup
    AlertPopup* popup = orxCRAFT_CAST<AlertPopup *>(
	    OrxCraft::GetInstance().GetDialogManager()->OpenDialog(
		"AlertPopup", _mWindow->getText().c_str()));
    orxASSERT(popup != orxNULL);
    popup->SetText(_mStPromptLoad);
}

/*************************************************************************
  CCEGUIFileDialog::_openPromptWindowSave
  Opens the prompt window in case of saving
 *************************************************************************/
void CEGUIFileDialog::_openPromptWindowSave (void)
{
    // Open popup
    ConfirmPopup* popup = orxCRAFT_CAST<ConfirmPopup *>(
	    OrxCraft::GetInstance().GetDialogManager()->OpenDialog(
		"ConfirmPopup", _mWindow->getText().c_str()));
    orxASSERT(popup != orxNULL);
    popup->SetText(_mStPromptSave);
}

void CEGUIFileDialog::OnAction(const string& widgetName, const string& action)
{
    if (widgetName == NAME_PUSHBUTTON_BACK) {
	// Back button pushed
	_handleBack();
    } else if (widgetName == NAME_PUSHBUTTON_CANCEL) {
	// Cancel button pushed
	_handleCancel();
    } else if (widgetName == NAME_PUSHBUTTON_OK) {
	// OK button pushed
	_handleOk();
    } else if (widgetName == NAME_LISTBOX_FILES) {
	// Event inside file list
	if(action == "MouseClick") {
	    // Single click - select file
	    _handleFileSelect();
	} else if (action == "MouseDoubleClick") {
	    // Double click - select directory
	    _handleDirectorySelect();
	} else {
	    orxASSERT(false);
	}
    } else {
	orxASSERT(false);
    }
}

void CEGUIFileDialog::OnInput(const string& widgetName)
{
    if (widgetName == NAME_COMBOBOX_EXTENSION) {
	// Extension selection changed
	_handleFilterSelect();
    } else if (widgetName == NAME_COMBOBOX_DRIVE) {
	// Drive selection changed
	_handleDriveSelect();
    } else if (widgetName == NAME_EDITBOX_FILE) {
	// File name changed
	_handleFileInput();
    } else if (widgetName == NAME_EDITBOX_PATH) {
	// Path changed
	_handlePathInput();
    }
}

void CEGUIFileDialog::HandlePopup(const string& popupName, orxU32 popupID)
{
    if(popupName == "AlertPopup") {
	// Nothing to do for the alert popup return
    } else if (popupName == "ConfirmPopup") {
	// Ask DialogManager for popup pointer
	ConfirmPopup* popup = orxCRAFT_CAST<ConfirmPopup *>(
		OrxCraft::GetInstance().GetDialogManager()->GetDialog(
		    popupID));
	orxASSERT(popup != orxNULL);
	if(popup->GetResult()) {
	    // Popup returned true - OK button was pushed
	    _handlePromptSaveOk();
	}
    } else {
	orxASSERT(false);
    }
}


/*************************************************************************
  CCEGUIFileDialog::_handleClose
  Close the window
 *************************************************************************/
bool CEGUIFileDialog::_handleClose (const CEGUI::EventArgs& e)
{
    _prefillOutputAndClearInput ();
    _currentOutput.SetAction (ACTION_WINDOW_CLOSED);
    _closeWindow ();
    return true;
}

/*************************************************************************
  CCEGUIFileDialog::_handleCancel
  Cancel Loading/saving and close the window
 *************************************************************************/
bool CEGUIFileDialog::_handleCancel ()
{
    _prefillOutputAndClearInput ();
    _currentOutput.SetAction (ACTION_CANCEL);
    _closeWindow ();
    return true;
}

/*************************************************************************
  CCEGUIFileDialog::_okExecute
 *************************************************************************/
void CEGUIFileDialog::_okExecute (void)
{
    _prefillOutputAndClearInput ();
    _Path* path = _getPathOfSelectedDrive ();
    if (path)
    {
	_currentOutput.SetFullQualifiedFileName (    path->szAbsolutePath + 
		SEPARATOR + 
		_getCurrentFile());
	_currentOutput.SetAbsolutePath (path->szAbsolutePath);
	_currentOutput.SetRelativePath (path->szRelativePath);
    }

    _currentOutput.SetFileName (_getCurrentFile());
    _currentOutput.SetFileExisting (_isFileExisting(_getCurrentFile()));
    _currentOutput.SetAction (ACTION_OK);

    _closeWindow ();
}

/*************************************************************************
  CCEGUIFileDialog::_handleOk
  Executed when OK has been pressed.
  An output object is created with the current settings or in case 
  prompt has been set, the prompt window is displayed
 *************************************************************************/
bool CEGUIFileDialog::_handleOk ()
{
    if (_currentInput.GetDoPrompt() &&
	    _currentInput.GetOpenFileDialog() &&
	    !_isFileExisting(_getCurrentFile()))
    {
	// We are in Load mode and file does not exist. Launch Alert Popup.
	_openPromptWindowLoad();
	return true;
    }

    if (_currentInput.GetDoPrompt() &&
	    !_currentInput.GetOpenFileDialog() &&
	    _isFileExisting(_getCurrentFile()))
    {
	// We are in Save mode and file does exist. Launch Confirm overwrite
	// Popup.
	_openPromptWindowSave();
	return true;
    }

    _okExecute ();
    return true;
}

/*************************************************************************
  CCEGUIFileDialog::_handleBack
  One step back in the directory tree
 *************************************************************************/
bool CEGUIFileDialog::_handleBack ()
{
    _adjustPathOfSelectedDrive (BRACKET_OPEN + ".." + BRACKET_CLOSED);
    _fillListbox ();
    return true;
}

/*************************************************************************
  CCEGUIFileDialog::_handleDriveSelect
  Executed when a drive is selected
 *************************************************************************/
bool CEGUIFileDialog::_handleDriveSelect ()
{
#ifdef __orxWINDOWS__
    // Change current path to the last one for the new selected drive
    _adjustPathOfSelectedDrive (BRACKET_OPEN + "." + BRACKET_CLOSED);
#else /* __orxWINDOWS__ */
    // For *nix systems we treat the drives as bookmarks. So do not retrieve
    // previous path inside users HOME, rather we return the path to HOME
    // itself.
    _adjustPathOfSelectedDrive (BRACKET_OPEN + "/" + BRACKET_CLOSED);
#endif
    _fillListbox ();
    return true;
}

/*************************************************************************
  CCEGUIFileDialog::_handleFilterSelect
  Executed when a filter (extension) is selected.
 *************************************************************************/
bool CEGUIFileDialog::_handleFilterSelect ()
{
    _fillListbox ();

    if (!_currentInput.GetOpenFileDialog())
    {
	// Dialog is open for 'Save As'. Only then the extension of the file is
	// changed after selecting an extension
	_Filter* filterElement = _getCurrentFilterElement();
	if (filterElement)
	{
	    // Get the extension without the "*."
	    string szExt = _stripFileExtension(filterElement->szExtension);

	    // Get the filename without the extension and set it in the editbox
	    string szFileName = _stripFileExtension(_getCurrentFile(), false);
	    string szRes = szFileName + "." + szExt;
	    _setCurrentFile (szRes);
	}
    }

    return true;
}

/*************************************************************************
  CCEGUIFileDialog::_handleFileSelect
  Executed when a filename (in the listbox) is selected
 *************************************************************************/
bool CEGUIFileDialog::_handleFileSelect ()
{
    if (_mLbxFiles)
    {
	const vector<string>& items = _mLbxFiles->GetSelection();
	if (!items.empty())
	{
	    // Return if a directory is selected
	    if (items[0][0] == BRACKET_OPEN_ASCII)
		return true;

	    if (_mEdtFile)
		_mEdtFile->SetText(items[0]);
	}
    }
    return true;
}

void CEGUIFileDialog::_handleFileInput ()
{
    if (_mEdtFile) {
	const string& name = _mEdtFile->GetText();
	// Select new file in the list if it exists
	if (_mLbxFiles && _mLbxFiles->HasItem(name)) {
	    vector<string> selection;
	    selection.push_back(name);
	    _mLbxFiles->SetSelection(selection);
	}
    }
}

void CEGUIFileDialog::_handlePathInput ()
{
    if (_mEdtPath) {
	const string& name = _mEdtPath->GetText();
	_adjustPathOfSelectedDrive (BRACKET_OPEN + name + BRACKET_CLOSED);
	_fillListbox ();
    }
}

/*************************************************************************
  CCEGUIFileDialog::_handleDirectorySelect
  Executed when a directory (in the listbox) is selected
 *************************************************************************/
bool CEGUIFileDialog::_handleDirectorySelect ()
{
    if (_mLbxFiles)
    {
	const vector<string>& items = _mLbxFiles->GetSelection();
	if (!items.empty())
	{
	    // Return if no directory is selected
	    if (items[0][0] != BRACKET_OPEN_ASCII)
		return true;

	    // Determine new directory
	    _adjustPathOfSelectedDrive (items[0]);
	    _fillListbox ();
	}
    }
    return true;
}


/*************************************************************************
  CCEGUIFileDialog::_handlePromptSaveOk
  Executed when the OK button in the prompt save window has been pushed
 *************************************************************************/
bool CEGUIFileDialog::_handlePromptSaveOk ()
{
    _okExecute ();
    return true;
}

/*************************************************************************
  CCEGUIFileDialog::_isFileExisting
  Determines whether a file exists in the selected path
 *************************************************************************/
bool CEGUIFileDialog::_isFileExisting (const string szFileName)
{
    string szPathPattern = _getPathOfSelectedDrive()->szAbsolutePath;
    string szName = szPathPattern + SEPARATOR + szFileName;
    return orxFile_Exists(szName.c_str());
}

/*************************************************************************
  CCEGUIFileDialog::_prefillOutputAndClearInput
  Fills the output object with generic data and clear the input data
 *************************************************************************/
void CEGUIFileDialog::_prefillOutputAndClearInput (void)
{
    _currentOutput.SetHandle (_currentInput.GetHandle());
    _currentOutput.SetFullQualifiedFileName ("");
    _currentOutput.SetDrive (_getCurrentDrive());
    _currentOutput.SetAbsolutePath ("");
    _currentOutput.SetRelativePath ("");
    _currentOutput.SetFileName ("");

    _currentInput.SetDefaultExtension ("");
    _currentInput.SetFileName ("");
    _currentInput.SetFilter ("");
    _currentInput.SetHandle (0);
    _currentInput.SetTitle ("");
}

/*************************************************************************
  CCEGUIFileDialog::_determineDrives
  Determine all active drives
 *************************************************************************/
void CEGUIFileDialog::_determineDrives (void)
{
    _drives.clear();

#ifdef __orxWINDOWS__

    string szDrive = "A:";
    // Bit mask of all drives in the system
    ulong uDriveMask = _getdrives();
    if (uDriveMask != 0)
    {
	while (uDriveMask)
	{
	    if (uDriveMask & 1)
		_drives.push_back (szDrive);

	    ++szDrive[0];
	    uDriveMask >>= 1;
	}
    }

#else /* __orxWINDOWS__ */

    // root directory
    string linux_root = "/";
    _drives.push_back (linux_root);

    // media directory
    string linux_media = "/media";
    _drives.push_back(linux_media);

    // home directory
    char *home;
    string linux_home ="/home" ;
    if ( (home = getenv("HOME")) != NULL )
	linux_home = home;
    _drives.push_back(linux_home);

    // current path
    _drives.push_back(_getWorkingPath());

#endif /* __orxWINDOWS__ */

    sort(_drives.begin(), _drives.end());
}

/*************************************************************************
  CCEGUIFileDialog::_stripBrackets
  Strip the left and right bracket of a string
 *************************************************************************/
string CEGUIFileDialog::_stripBrackets(const string& szItem)
{
    string szNewItem = "";
    if (szItem.length() > 1)
    {
	szNewItem.assign (szItem, 1, szItem.length() - 2);
    }
    return szNewItem;
}

/*************************************************************************
  CCEGUIFileDialog::_stripDrive
  Returns the drive of a full qualified path
 *************************************************************************/
string CEGUIFileDialog::_stripDrive(const string& szItem)
{
    vector<string>::reverse_iterator it;
    // Iterate drives in revers order. For *nix systems this will match longest
    // bookmarks first
    for(it = _drives.rbegin(); it != _drives.rend(); it++) {
	if(szItem.find(*it) == 0) {
	    // Path starts with this drive name. Return it.
	    return *it;
	}
    }

    // Path does not begin with any known drive name. Return empty string.
    return "";
}

/*************************************************************************
  CCEGUIFileDialog::_stripFileExtension
  Strips the extension of a file and returns either:
  - The extension (returnRightPart = true; the default)
  - The filename without extension (returnRightPart = false)
 *************************************************************************/
string CEGUIFileDialog::_stripFileExtension(const string& szItem, bool bReturnRightPart)
{
    if (szItem.length() == 0)
	return "";

    string szExt;
    string::size_type pos = szItem.rfind (".", szItem.length());
    if (pos != string::npos)
    {
	if (bReturnRightPart)
	{
	    pos++;
	    return szExt.assign (szItem, pos, szItem.length() - pos);
	}
	else
	{
	    return szExt.assign (szItem, 0, pos);
	}
    }

    return "";
}

/*************************************************************************
  CCEGUIFileDialog::_matchItem (string)
  Determines whether an item (directory or filename) matches the
  selected filter element.
 *************************************************************************/
bool CEGUIFileDialog::_matchItem (const string& szItem)
{
    // If the item doesn't have an extension, false is returned
    if (szItem.length() == 0)
	return false;

    // Return true if it is a directory
    if (szItem.length() > 0)
	if (szItem[0] == BRACKET_OPEN_ASCII)
	    return true;

    // Return true if all files should be displayed
    _Filter* filterElement = _getCurrentFilterElement();
    if (filterElement)
    {
	if (filterElement->szExtension == EXTENSION_ALL)
	    return true;

	// Do they match?
	string szExt = "*." + _stripFileExtension (szItem);
	if (szExt == filterElement->szExtension)
	    return true;
    }

    return false;
}

/*************************************************************************
  CCEGUIFileDialog::_getWorkingPath
  Get the current working directory (full qualified)
 *************************************************************************/
string CEGUIFileDialog::_getWorkingPath (void)
{
    char* currentPath = new char[FILENAME_MAX];
#ifdef __orxWINDOWS__
    if ( _getcwd(currentPath, FILENAME_MAX ) != NULL )
#else /* __orxWINDOWS__ */
	if ( getcwd(currentPath, FILENAME_MAX ) != NULL )
#endif /* __orxWINDOWS__ */
	{
	    return currentPath;
	}

    return "";
}

/*************************************************************************
  CCEGUIFileDialog::_getCurrentDrive
  Get the current drive. This can be:
  - The selected drive
  - If no drive selected, the drive of the working directory is returned
 *************************************************************************/
string CEGUIFileDialog::_getCurrentDrive (void)
{
    // Return the selected drive (if available)
    if (_mCmbDrives)
    {
	const string& item = _mCmbDrives->GetSelection();
	if (!item.empty())
	{
	    return item;
	}
    }

    // If no drive was selected, use the one from the working directory
    return _stripDrive (_getWorkingPath());
}

void CEGUIFileDialog::_setCurrentDrive (const string& drive)
{
    if(find(_drives.begin(), _drives.end(), drive) != _drives.end())
    {
	_mCmbDrives->SetSelection(drive);
    }
}

/*************************************************************************
  CCEGUIFileDialog::_getCurrentFilterElement
  Get the current filter element. This can be:
  - The selected one from the extensions combobox
  - If no one selected, the first one from the list is returned
 *************************************************************************/
CEGUIFileDialog::_Filter* CEGUIFileDialog::_getCurrentFilterElement (void)
{
    // Return the selected one  (if available)
    if (_mCmbExtensions)
    {
	const string& item_name = _mCmbExtensions->GetSelection();
	_Filter* item = _filterParser.getParsedFilter(item_name);
	if (item)
	{
	    return item;
	}

	// If no one was selected, use the one which is presented
	/* Do we need this ???
	string szText = _mCmbExtensions->getText ().c_str ();
	uint uNumberOfEntries = _mCmbExtensions->getItemCount();
	for (uint uPos = 0; uPos < uNumberOfEntries; uPos++)
	{
	    _FilterElement* item = static_cast <_FilterElement*>(_mCmbExtensions->getListboxItemFromIndex (uPos));
	    if (item)
	    {
		if (szText == item->szDescription)
		    return item;
	    }
	}
	*/

	// If nothing displayed, use the first from the list
	/* Why we would end up here??
	if (uNumberOfEntries)
	{
	    _FilterElement* item = static_cast <_FilterElement*>(_mCmbExtensions->getListboxItemFromIndex (0));
	    if (item)
		return item;
	}
	*/
    }

    return NULL;
}

/*************************************************************************
  CCEGUIFileDialog::_setCurrentFile
  Set a filename in the files editbox.
 *************************************************************************/
void CEGUIFileDialog::_setCurrentFile (const string& szNewFileName)
{
    if (_mEdtFile)
    {
	// Change the currently displayed text
	_mEdtFile->SetText(szNewFileName);
    }
}

/*************************************************************************
  CCEGUIFileDialog::_getCurrentFile
  Get the filename from the editbox.
 *************************************************************************/
string CEGUIFileDialog::_getCurrentFile (void)
{
    // Return the selected file from the combobox (if available)
    if (_mEdtFile)
    {
	return _mEdtFile->GetText();
    }

    return "";
}


/*************************************************************************
  CCEGUIFileDialog::_adjustPathOfSelectedDrive
  Changes the directory of the selected drive.
 *************************************************************************/
void CEGUIFileDialog::_adjustPathOfSelectedDrive(
	const string& szDirectoryChange)
{
    // Get current path
    _Path tmp_path = *_getPathOfSelectedDrive ();
    string szDir = _stripBrackets (szDirectoryChange);
    if (szDir == "..")
    {
	// Go one step back
	string::size_type pos = tmp_path.szAbsolutePath.rfind (SEPARATOR,
		tmp_path.szAbsolutePath.length());

	if (pos == 0)
	{
	    // We end up at root directory. Set it explicitely as otherwise we
	    // end up with emtpy string that will be treated by real path as
	    // current dir
	    tmp_path.szAbsolutePath = SEPARATOR;
	}
	else if (pos != string::npos)
	{
	    // Copy path up to last separator
	    tmp_path.szAbsolutePath.assign (tmp_path.szAbsolutePath, 0, pos);
	}
    }
    else if (szDir == "/")
    {
	// Go to the root
	tmp_path.szAbsolutePath = _getCurrentDrive();
    }
    else if (szDir != ".")
    {
	if(_stripDrive(szDir) != "")
	{
	    // Path starts with known drive (bookmark in *nix) - this is an
	    // absolute path. Use it literally
	    tmp_path.szAbsolutePath = szDir;
	}
	else
	{
	    // Must be a directory step, update the path
	    tmp_path.szAbsolutePath = tmp_path.szAbsolutePath + SEPARATOR + szDir;
	}
    }
    // Else would be "." - current directory in which case we do nothing

#ifdef __orxWINDOWS__
    // TODO Use GetFullPathName to normalize path and then some other function
    // to check that file exists.
#else
    // Normalize the new path, check that it exists
    char* real_path = realpath(tmp_path.szAbsolutePath.c_str(), NULL);
    if(real_path == NULL)
    {
	// Path does not exits. Reset the edit field to current path and return.
	free(real_path);
	_Path* path = _getPathOfSelectedDrive ();
	_mEdtPath->SetText(path->szAbsolutePath);
	return;
    }
    tmp_path.szAbsolutePath = real_path;
    free(real_path);
#endif /* ! __orxWINDOWS__ */

    // Set new drive
    if(_stripDrive(tmp_path.szAbsolutePath) != "")
	_setCurrentDrive(_stripDrive(tmp_path.szAbsolutePath));
    // Adjust its path
    _Path* path = _getPathOfSelectedDrive ();
    path->szAbsolutePath = tmp_path.szAbsolutePath;

    string szDrive = _getCurrentDrive();
    string szCurrentPath = _getWorkingPath();

    // In case the drive of the working path is identical to the drive of the
    // absolute path or in case of file system with global root adjust the
    // relative path to current working directory
    if (szDrive == _stripDrive(szCurrentPath) || global_file_tree)
    {
	path->szRelativePath = "";
	string szBranchWorkingPath = "";
	string szBranchAbsolutePath = "";
	string szRelative = "";
	uint uLenCurrent = szCurrentPath.length();
	uint uLenAbs = path->szAbsolutePath.length();
	uint u = 0;

	// Determine where the working path and the absolute path are branching
	while (szCurrentPath[u] == path->szAbsolutePath[u] && u < uLenCurrent && u < uLenAbs)
	    u++;

	// Go back until the first separator is detected
	if (u < uLenCurrent)
	    while (szCurrentPath[u] != SEPARATOR[0] && u != 0)
		u--;

	// Get the branch of the working path
	szBranchWorkingPath.assign (szCurrentPath, u, szCurrentPath.length() - u);

	// Get the branch of the absolute path (increase counter by 1 to prevent duplicate separators)
	if (u < uLenAbs)
	    u++;
	szBranchAbsolutePath.assign (path->szAbsolutePath, u, path->szAbsolutePath.length() - u);

	// The amount of separators determines the steps back.
	uint uLenBranchWorkingPath = szBranchWorkingPath.length();
	for (u = 0; u < uLenBranchWorkingPath; u++)
	{
	    if (szBranchWorkingPath[u] == SEPARATOR[0])
		szRelative = szRelative + ".." + SEPARATOR;
	}

	// Concatenate the 2 parts
	path->szRelativePath = szRelative + szBranchAbsolutePath;
    }

    // Update the edit box
    _mEdtPath->SetText(path->szAbsolutePath);
}

/*************************************************************************
  CCEGUIFileDialog::_getPathOfSelectedDrive
  Returns the path of the selected drive.
 *************************************************************************/
CEGUIFileDialog::_Path* CEGUIFileDialog::_getPathOfSelectedDrive (void)
{
    string szDrive = _getCurrentDrive();
    _Path* path = NULL;

    std::map<string, _Path*>::iterator it;
    it = _currentPaths.find(szDrive);

    if (it != _currentPaths.end())
    {
	// Found it
	return it->second;
    }
    else
    {
	// No path found, add a new one
	path = new _Path ("");
	string szCurrentPath = _getWorkingPath();
	if (szDrive == _stripDrive(szCurrentPath))
	    path->szAbsolutePath = szCurrentPath;
	else
	    // TODO Windows version ??
	    path->szAbsolutePath = szDrive;

	_currentPaths.insert(make_pair(szDrive, path));
    }

    return path;
}

/*************************************************************************
  CCEGUIFileDialog::_fillListbox
  Fill the listbox with files
 *************************************************************************/
void CEGUIFileDialog::_fillListbox  (void)
{
    string szName;
    string szPathPattern;

    if (_mLbxFiles)
    {
	// Pattern to search for files
	szPathPattern = _getPathOfSelectedDrive()->szAbsolutePath;
	szPathPattern = szPathPattern + SEPARATOR + "*";
	vector<string> files; // Found files
	vector<string> dirs; // Found dirs
	orxFILE_INFO c_file; // Orx file info struct

	// Find first file/dir in search pattern
	if(orxFile_FindFirst(szPathPattern.c_str(), &c_file))
	{
	    szName = string(c_file.zName);
	    if (c_file.u32Flags & orxFILE_KU32_FLAG_INFO_DIR)
	    {
		// This is a dir add it to the listbox in brackets
		szName = BRACKET_OPEN + szName + BRACKET_CLOSED;
		dirs.push_back(szName);
	    }
	    // Add it to the listbox if it passes the filter
	    else if (_matchItem(szName))
		files.push_back(szName);
	} else {
	    // orxDEBUG_PRINT(orxDEBUG_LEVEL_USER, "Directory is empty."); 
	}

	// Get the other ones
	while( orxFile_FindNext( &c_file ) )
	{
	    szName = string(c_file.zName);
	    if (c_file.u32Flags & orxFILE_KU32_FLAG_INFO_DIR)
	    {
		// This is a dir add it to the listbox in brackets
		szName = BRACKET_OPEN + szName + BRACKET_CLOSED;
		dirs.push_back(szName);
	    }
	    // Add it to the listbox if it passes the filter
	    else if (_matchItem(szName))
		files.push_back(szName);
	}

	// Close search
	orxFile_FindClose(&c_file);

	// Sort the entries
	sort(dirs.begin(), dirs.end());
	sort(files.begin(), files.end());
	// Merge dirs and files - dirs go first
	dirs.reserve(dirs.size() + distance(files.begin(), files.end()));
	dirs.insert(dirs.end(), files.begin(), files.end());

	// Fill the listbox
	_mLbxFiles->Fill(dirs);
    }
}

/*************************************************************************
  CCEGUIFileDialog::openDialog
  Call the file dialog
 *************************************************************************/
void CEGUIFileDialog::OpenDialog (const FileDialogInput& input)
{
    if (_mWindow)
    {
	// Store the input
	bool bDefaultExtensionSet = false;
	_currentInput = input;

	// Parse the filter and fill the filter elements (extensions) in the combobox
	_filterParser.parseFilter (input.GetFilter());
	map<string, _Filter*> filter = _filterParser.getParsedFilter();
	if (_mCmbExtensions && filter.size() > 0)
	{
	    // Loop through the filter and add the elements to the combobox
	    vector<string> names;
	    map<string, _Filter*>::iterator pos;
	    for (pos = filter.begin(); pos != filter.end(); pos++)
	    {
		if (pos->second)
		{
		    names.push_back(pos->second->szDescription);
		}
	    }

	    _mCmbExtensions->Fill(names);

	    // If the default extension in the input object isn't available; set the first in the list
	    if (!bDefaultExtensionSet)
	    {
		_Filter* filterElement = _filterParser.getParsedFilter(input.GetDefaultExtension());
		if (filterElement)
		{
		    _mCmbExtensions->SetSelection(input.GetDefaultExtension());
		}
		else
		{
		    _Filter* filterElement = _filterParser.getParsedFilter().begin()->second;
		    if (filterElement)
			_mCmbExtensions->SetSelection(filterElement->szDescription);
		}
	    }

	    // Fill the list
	    _fillListbox ();
	}

	// Set the title
	if (input.GetTitle() != "")
	{
	    _mWindow->setText(input.GetTitle());
	}
	else
	{
	    if (_currentInput.GetOpenFileDialog())
	    {
		_mWindow->setText(DEFAULT_TEXT_DIALOG_LOAD);
	    }
	    else
	    {
		_mWindow->setText(DEFAULT_TEXT_DIALOG_SAVE);
	    }
	}

	// Set the text in the prompt window
	if (input.GetDoPrompt())
	{
	    if (input.GetPromptText() != "")
	    {
		if (_currentInput.GetOpenFileDialog())
		    _mStPromptLoad = input.GetPromptText();
		else
		    _mStPromptSave = input.GetPromptText();
	    }
	    else
	    {
		if (_currentInput.GetOpenFileDialog())
		    _mStPromptLoad = DEFAULT_TEXT_PROMPT_LOAD;
		else
		    _mStPromptSave = DEFAULT_TEXT_PROMPT_SAVE;
	    }
	}

	// Display the (default) filename
	_mEdtFile->SetText(input.GetFileName());

	// Show the dialog
	_mWindow->show();
	_mWindow->activate();
    }
}

/*************************************************************************
  CCEGUIFileDialog::_FilterParser::__deleteFilter
  Delete the vector with filter elements
 *************************************************************************/
void CEGUIFileDialog::_FilterParser::__deleteFilters()
{
    map<string, _Filter*>::iterator pos;
    for (pos = _filters.begin(); pos != _filters.end(); pos++)
    {
	if (pos->second)
	    delete pos->second;
    }

    _filters.clear();
}

/*************************************************************************
  CCEGUIFileDialog::_FilterParser::parseFilter
  Parse the filter string. The string is divided and the parsed data
  is added to _Filter objects
 *************************************************************************/
void CEGUIFileDialog::_FilterParser::parseFilter(const string& szFilter)
{
    __deleteFilters();
    _Filter* filter;
    string szPart;
    bool push = false;
    string::size_type idxStart = 0;
    string::size_type idxEnd = szFilter.find ("|", idxStart);
    while (idxEnd != string::npos)
    {
	szPart.assign (szFilter, idxStart, idxEnd - idxStart);
	if (!push)
	{
	    filter = new _Filter();
	    if (filter)
		filter->szDescription = szPart;
	    push = true;
	}
	else
	{
	    if (filter)
	    {
		filter->szExtension = szPart;
		_filters.insert(make_pair(filter->szDescription, filter));
		push = false;
	    }
	}

	idxStart = ++idxEnd;
	idxEnd = szFilter.find ("|", idxStart);
    }
}

map<string, CEGUIFileDialog::_Filter*> CEGUIFileDialog::_FilterParser::getParsedFilter(void)
{
    return _filters;
}

CEGUIFileDialog::_Filter* CEGUIFileDialog::_FilterParser::getParsedFilter(
	const string& description)
{
    std::map<string, _Filter*>::iterator it;
    it = _filters.find (description);
    if (it != _filters.end())
    {
	// Found it
	return it->second;
    }

    return NULL;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
