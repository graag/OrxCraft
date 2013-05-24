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

#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

/**
 * @file FileDialog.h
 * @date 2013-04-19
 * @author graag@o2.pl
 *
 * File picker interface.
 * Based on code written by spookyboo ported to Linux by ancestral:
 * http://www.ogre3d.org/tikiwiki/tiki-index.php?page=ceguicommonfiledialog
 */

#include <string>

#include "ScrollFrameWindow.h"

const std::string EXTENSION_ALL                = "*.*";
const std::string EXTENSION_PRESENTATION_DEFAULT        = "All files (*.*)|*.*|";

/** Type of action executed by user. */
enum FileDialogAction
{
    ACTION_OK,           // Dialog window closed after OK button clicked
    ACTION_CANCEL,       // Dialog window closed after CANCEL button clicked
    ACTION_WINDOW_CLOSED // Dialog window closed with "x" button
};

/** Class describing data structure of FileDialog input. */
class FileDialogInput
{
public:
    /** C-tor
     *
     * @param[in] uHandle - Handle that will identify FileDialog caller
     * @param[in] szFileName - File name
     * @param[in] szFilter - Filter for allowed extensions.
     *            The filter is build according to the windows common file
     *            dialog filter string. An example: The filter
     *
     *              "HTML Files (*.htm)|*.htm|Active Server Pages (*.asp)|"
     *              "*.asp|Perl Script (*.pl)|*.pl|All files|*.*|"
     *              (don't forget the last | or otherwise the last element will
     *              not be parsed)
     *
     *            produces a list that looks like this in the combobox:
     *
     *              HTML Files (*.htm)
     *              Active Server Pages (*.asp)
     *              Perl Script (*.pl)
     *              All files
     *
     * @param[in] szDefaultExtension - Default extension
     * @param[in] szTitle - Title of the window:
     *            Default title is "Load" in case _bOpenFileDialog is TRUE.
     *            Default title is "Save as" in case _bOpenFileDialog is FALSE.
     * @param[in] bOpenFileDialog - TRUE for FileLoad, FALSE for FileSaveAs
     * @param[in] bDoPrompt - TRUE for prompting (are you sure?), FALSE for
     *            non-prompting
     * @param[in] szPromptText - Define text in the prompt window
     *            Default for Load: "Cannot find the requested file."
     *            Default for Save: "The file already exist. Replace it?"
     */
    FileDialogInput (    const orxU32 uHandle = 0,
	    const std::string& szFileName = "",
	    const std::string& szFilter = EXTENSION_PRESENTATION_DEFAULT,
	    const std::string& szDefaultExtension = EXTENSION_ALL,
	    const std::string& szTitle = "",
	    bool bOpenFileDialog = true,
	    bool bDoPrompt = true,
	    const std::string& szPromptText = "") :
	_uHandle (uHandle),
	 _szFileName (szFileName),
	 _szFilter (szFilter),
	 _szDefaultExtension (szDefaultExtension),
	 _szTitle (szTitle),
	 _bOpenFileDialog (bOpenFileDialog),
	 _bDoPrompt (bDoPrompt),
	 _szPromptText (szPromptText){};

    /** D-tor */
    virtual ~FileDialogInput(void){};
    /** Return dialog handle - id used by the caller. */
    orxU32 GetHandle (void) const;
    /** Get selected file name. */
    std::string GetFileName (void) const;
    /** Get filter string. */
    std::string GetFilter (void) const;
    /** Get default extension. */
    std::string GetDefaultExtension (void) const;
    /** Get window title. */
    std::string GetTitle (void) const;
    /** TRUE for FileLoad, FALSE for FileSaveAs */
    bool GetOpenFileDialog (void) const;
    /** TRUE for prompting (are you sure?), FALSE for non-prompting */
    bool GetDoPrompt (void) const;
    /** Get prompt message. */
    std::string GetPromptText (void) const;

    /** Set dialog handle - id used by the caller. */
    void SetHandle (const orxU32);
    /** Set selected file name. */
    void SetFileName (const std::string&);
    /** Set filter string. */
    void SetFilter (const std::string&);
    /** Set default extension. */
    void SetDefaultExtension (const std::string&);
    /** Set window title. */
    void SetTitle (const std::string&);
    /** Set to TRUE for FileLoad, FALSE for FileSaveAs */
    void SetOpenFileDialog (const bool);
    /** Set to TRUE for prompting (are you sure?), FALSE for non-prompting */
    void SetDoPrompt (const bool);
    /** Set prompt message. */
    void SetPromptText (const std::string&);

private:
    /** Handle, that is supplied by the calling function. This handle serves as
     * an identifier that determines the 'caller'.  It will be added to the
     * output object 'FileDialogOutput' after the Ok button in the
     * FileDialog has been pressen, so the triggered function (registered
     * by means of "subscribe") is able to identify who has originally called
     * the dialog. Default: 0 (no handle)
     */
    orxU32 _uHandle;
    /** Filename.
     * Default: "" (no filename)
     */
    std::string _szFileName;
    /** Filter for allowed extensions.
     * Default: "All files|*.*|"
     * The filter is build according to the windows common file dialog filter
     * string. An example: The filter
     *
     *   "HTML Files (*.htm)|*.htm|Active Server Pages (*.asp)|"
     *   "*.asp|Perl Script (*.pl)|*.pl|All files|*.*|"
     *   (don't forget the last | or otherwise the last element will not be
     *   parsed)
     *
     * produces a list that looks like this in the combobox:
     *
     *   HTML Files (*.htm)
     *   Active Server Pages (*.asp)
     *   Perl Script (*.pl)
     *   All files
     */
    std::string _szFilter;
    /** Default Extension
     * Default: *.*
     */
    std::string _szDefaultExtension;
    /** Title of the window.
     * Default title is "Load" in case _bOpenFileDialog is TRUE.
     * Default title is "Save as" in case _bOpenFileDialog is FALSE.
     */
    std::string _szTitle;
    /** TRUE for FileLoad, FALSE for FileSaveAs
     * Default: TRUE
     */
    bool _bOpenFileDialog;
    /** TRUE for prompting (are you sure?), FALSE for non-prompting
     * Default: TRUE
     */
    bool _bDoPrompt;
    /** Define text in the prompt window
     * Default for Load: "Cannot find the requested file."
     * Default for Save: "The file already exist. Replace it?"
     */
    std::string _szPromptText;
};

/** Class describing data structure of FileDialog output. */
class FileDialogOutput
{
public:
    /** C-tor
     *
     * @param[in] uHandle - handle used by the called when opening FileDialog
     * @param[in] szFullQualifiedFilename - selected absolute path
     * @param[in] szDrive - selected drive
     * @param[in] szRelativePath - relative path with regards to working dir
     * @param[in] szFileName - file name
     * @param[in] bFileExisting - does the file exist
     */
    FileDialogOutput (const orxU32 uHandle = 0,
	    const std::string& szFullQualifiedFileName = "",
	    const std::string& szDrive = "",
	    const std::string& szAbsolutePath = "",
	    const std::string& szRelativePath = "",
	    const std::string& szFileName = "",
	    const bool bFileExisting = false) : 
	_uHandle (uHandle),
	_szFullQualifiedFileName (szFullQualifiedFileName),
	_szDrive (szDrive),
	_szAbsolutePath (szAbsolutePath),
	_szRelativePath (szRelativePath),
	_szFileName (szFileName),
	_bFileExisting(bFileExisting){};

    /** D-tor */
    virtual ~FileDialogOutput(void){};
    /** Return dialog handle - id used by the caller. */
    orxU32 GetHandle (void) const;
    /** Return the file name with absolute path. */
    std::string GetFullQualifiedFileName (void) const;
    /** Get selected drive. */
    std::string GetDrive (void) const;
    /** Get files absolute path. */
    std::string GetAbsolutePath (void) const;
    /** Get files relative path if it is on current drive */
    std::string GetRelativePath (void) const;
    /** Get selected file name. */
    std::string GetFileName (void) const;
    /** Return user action. */
    FileDialogAction GetAction (void) const;
    /** Check that file exists. */
    bool IsFileExisting (void);
    /** Set dialog handle - id used by the caller. */
    void SetHandle (const orxU32);
    /** Return the file name with absolute path. */
    void SetFullQualifiedFileName (const std::string&);
    /** Set selected drive. */
    void SetDrive (const std::string&);
    /** Set files absolute path. */
    void SetAbsolutePath (const std::string&);
    /** Set files relative path */
    void SetRelativePath (const std::string&);
    /** Set selected file name. */
    void SetFileName (const std::string&);
    /** Set user action. */
    void SetAction (FileDialogAction);
    /** Set if the file exists. */
    void SetFileExisting (const bool);

private:
    /** Handle, that is supplied by the calling function. This
     * handle serves as an identifier that determines the 'caller'. It will be
     * added to the output object 'FileDialogOutput' after the Ok button
     * in the FileDialog has been pressed, so the triggered function
     * (registered by means of "subscribe") is able to identify who has
     * originally called the dialog. Default: 0 (no handle)
     */
    orxU32 _uHandle;
    //! File name including drive and (absolute) path.
    std::string _szFullQualifiedFileName;
    //! The selected drive.
    std::string _szDrive;
    //! The absolute path, with drive and without filename.
    std::string _szAbsolutePath;
    //! The relative path (only set if the working directory is in the selected
    //! drive).
    std::string _szRelativePath;
    //! File name.
    std::string _szFileName;
    //! Determines how the dialog was ended.
    FileDialogAction _action;
    //! TRUE if file exists FALSE otherwise
    bool _bFileExisting;
};

/** Class defining interface of a file selection dialog window.
 *
 * Dialog window can operate in two modes - Save and Load. In the Save mode
 * user can specify non existing file. For existing files user will be
 * presented with a overwrite confirmation popup. In the load mode selection of
 * non existent file will result in an Alert popup window.
 */
class FileDialog : public ScrollFrameWindow
{
public:
    /** C-tor
     * @param[in] name - name of the dialog
     * @param[in] title - string with dialog title, if not specified the
     *                      dialog name is used as a title. The title
     *                      identifies different instaces of the same dialog
     *                      type.
     */
    FileDialog(const std::string& name, const std::string& title = "");

    /** Open the file dialog window.
     *
     * @param[in] input - contents to be displayed in the dialog window.
     */
    virtual void OpenDialog (const FileDialogInput& input) = 0;
    /** Get the results of user action in the dialog window.
     *
     * @return - object describing user decision.
     */
    virtual const FileDialogOutput& GetResult (void);


    /* ScrollFrameWindow interface */

    virtual void OnClose();
    virtual void OnReset() { OnClose(); }
    virtual void HandlePopup(const std::string& popupName, orxU32 popupID);
    virtual void HandleClose(const std::string& popupName, orxU32 popupID);

    /* Signals */

    /** Notify caller on popup finish. */
    Gallant::Signal2<const std::string&, orxU32> SignalFinish;

protected:
    //! The input data, set after each call (function 'openDialog') is stored
    FileDialogInput _currentInput;
    //! The output data
    FileDialogOutput _currentOutput;
};
#endif // __FILEDIALOG_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
