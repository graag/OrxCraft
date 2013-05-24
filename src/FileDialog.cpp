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
 * @file FileDialog.cpp
 * @date 2013-04-19
 * @author graag@o2.pl
 *
 * File picker interface.
 * Based on code written by spookyboo ported to Linux by ancestral:
 * http://www.ogre3d.org/tikiwiki/tiki-index.php?page=ceguicommonfiledialog
 */

#include "FileDialog.h"

#include <string>

#include "orx.h"
#include "orxCraft.h"
#include "DialogManager.h"

using std::string;

/*************************************************************************
  FileDialogInput::Getters & Setters
 *************************************************************************/
orxU32 FileDialogInput::GetHandle (void) const
{
    return _uHandle;
}

string FileDialogInput::GetFileName (void) const
{
    return _szFileName;
}

string FileDialogInput::GetFilter (void) const
{
    return _szFilter;
}

string FileDialogInput::GetDefaultExtension (void) const
{
    return _szDefaultExtension;
}

string FileDialogInput::GetTitle (void) const
{
    return _szTitle;
}

bool FileDialogInput::GetOpenFileDialog (void) const
{
    return _bOpenFileDialog;
}

bool FileDialogInput::GetDoPrompt (void) const
{
    return _bDoPrompt;
}

string FileDialogInput::GetPromptText (void) const
{
    return _szPromptText;
}

void FileDialogInput::SetHandle (orxU32 uHandle)
{
    _uHandle = uHandle;
}

void FileDialogInput::SetFilter (const string& szFilter)
{
    _szFilter = szFilter;
}

void FileDialogInput::SetDefaultExtension (const string& szDefaultExtension)
{
    _szDefaultExtension = szDefaultExtension;
}

void FileDialogInput::SetFileName (const string& szFileName)
{
    _szFileName = szFileName;
}

void FileDialogInput::SetTitle (const string& szTitle)
{
    _szTitle = szTitle;
}

void FileDialogInput::SetOpenFileDialog (const bool bOpenFileDialog)
{
    _bOpenFileDialog = bOpenFileDialog;
}

void FileDialogInput::SetDoPrompt (const bool bDoPrompt)
{
    _bDoPrompt = bDoPrompt;
}

void FileDialogInput::SetPromptText (const string& szPromptText)
{
    _szPromptText = szPromptText;
}

/*************************************************************************
  FileDialogOutput::Getters & Setters
 *************************************************************************/
orxU32 FileDialogOutput::GetHandle (void) const
{
    return _uHandle;
}

string FileDialogOutput::GetFullQualifiedFileName (void) const
{
    return _szFullQualifiedFileName;
}

string FileDialogOutput::GetDrive (void) const
{
    return _szDrive;
}

string FileDialogOutput::GetAbsolutePath (void) const
{
    return _szAbsolutePath;
}

string FileDialogOutput::GetRelativePath (void) const
{
    return _szRelativePath;
}

string FileDialogOutput::GetFileName (void) const
{
    return _szFileName;
}

FileDialogAction FileDialogOutput::GetAction (void) const
{
    return _action;
}

bool FileDialogOutput::IsFileExisting (void)
{
    return _bFileExisting;
}

void FileDialogOutput::SetHandle (const orxU32 uHandle)
{
    _uHandle = uHandle;
}

void FileDialogOutput::SetFullQualifiedFileName (const string& szFullQualifiedFileName)
{
    _szFullQualifiedFileName = szFullQualifiedFileName;
}

void FileDialogOutput::SetDrive (const string& szDrive)
{
    _szDrive = szDrive;
}

void FileDialogOutput::SetAbsolutePath (const string& szAbsolutePath)
{
    _szAbsolutePath = szAbsolutePath;
}

void FileDialogOutput::SetRelativePath (const string& szRelativePath)
{
    _szRelativePath = szRelativePath;
}

void FileDialogOutput::SetFileName (const string& szFileName)
{
    _szFileName = szFileName;
}

void FileDialogOutput::SetAction (FileDialogAction action)
{
    _action = action;
}

void FileDialogOutput::SetFileExisting (bool bFileExisting)
{
    _bFileExisting = bFileExisting;
}

/***********
  FileDialog
 ***********/

FileDialog::FileDialog(const string& name, const string& title):
    ScrollFrameWindow(name, title)
{
    // Popups should be modal
    m_isModal = true;
}

const FileDialogOutput& FileDialog::GetResult (void)
{
    return _currentOutput;
}

void FileDialog::HandlePopup(const string& popupName,
	orxU32 popupTitle)
{
    orxASSERT (false);
}

void FileDialog::HandleClose(const string& popupName,
	orxU32 popupTitle)
{
    orxASSERT (false);
}

void FileDialog::OnClose()
{
    SignalClose(m_name, m_id);
    OrxCraft::GetInstance().GetDialogManager()->CloseDialog(m_id);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
