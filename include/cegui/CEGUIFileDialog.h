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

#ifndef __CEGUIFILEDIALOG_H__
#define __CEGUIFILEDIALOG_H__

/**
 * @file CEGUIFileDialog.h
 * @date 2013-04-19
 * @author graag@o2.pl
 *
 * CEGUI File picker implementation.
 * Based on code written by spookyboo ported to Linux by ancestral:
 *
 */

#include "CEGUI.h"

#include "FileDialog.h"
#include <map>

class CEGUICombobox;
class CEGUIListbox;

/** CEGUI implementation of FileDialog widget. */
class CEGUIFileDialog : public FileDialog
{
public:
    /** C-tor
     * @param[in] name - name of the dialog
     * @param[in] title - string with dialog title, if not specified the dialog
     *            name is used as a title. The title identifies different
     *            instaces of the same dialog type.
     */
    CEGUIFileDialog(const std::string& name, const std::string& title = "");
    /** D-tor */
    virtual ~CEGUIFileDialog(void);
    /** Open the file dialog window.
     *
     * @param[in] input - FileDialogInput object reference with dialog contents
     * description.
     */
    virtual void OpenDialog (const FileDialogInput& input);

    /* ScrollFrameWindow interface */

    virtual void Init ();
    virtual void HandlePopup(const std::string& popupName, orxU32 popupID);
    // Handlers
    virtual void OnAction(const std::string& widgetName,
	    const std::string& action = "");
    virtual void OnInput(const std::string& widgetName);

private:
    /*
     * Inner class that represents drive, path and filename
     */
    class _Path
    {
	public:
	    /** C-tor
	     *
	     * @param[in] szAbsolutePath - Aboslute path
	     */
	    _Path(const std::string& szAbsolutePath) :
		szAbsolutePath(szAbsolutePath),
		szRelativePath("") {};
	    /** D-tor */
	    virtual ~_Path(void){};
	    // Absolute path
	    std::string szAbsolutePath;
	    // Relative path
	    std::string szRelativePath;
    };

    /*
     * Inner class representing a filter element in a more appropriate way
     */
    class _Filter
    {
	public:
	    /** C-tor */
	    _Filter(void) {};
	    /** C-tor
	     *
	     * @param[in] szDescription - Filter description, will be displayed
	     *            in the GUI
	     * @param[in] szExtension - Filter expression used to filter files
	     */
	    _Filter(const std::string& szDescription,
		    const std::string& szExtension) :
		szDescription(szDescription),
		szExtension(szExtension) {};
	    /** D-tor */
	    virtual ~_Filter(void){};
	    // Description presented in the combobox
	    std::string szDescription;
	    // File extension
	    std::string szExtension;
    };

    /*
     * Inner class that parses the filter
     */
    class _FilterParser
    {
	public:
	    /** C-tor */
	    _FilterParser(void) {};
	    /** D-tor */
	    virtual ~_FilterParser(void){ __deleteFilters(); };
	    /** Parse filter string into individual filters.
	     *
	     * @param[in] input - string describing filters of the form:
	     *            <description1>|<ext1>|<description2>|<ext2>
	     */
	    void parseFilter(const std::string& input);
	    /** Return defined filters.
	     *
	     * @return Map of pointers to _Filer objects defined by call to
	     *         parseFilter
	     */
	    std::map<std::string, _Filter*> getParsedFilter(void);
	    /** Return specific filter
	     *
	     * @param[in] description - Filter description to return
	     * @return A pointer to _Filter object identified by specified
	     *         description.
	     */
	    _Filter* getParsedFilter(const std::string& description);
	private:
	    // Map of _Filter objects
	    std::map<std::string, _Filter*> _filters;
	    // Remove defined filers
	    void __deleteFilters();
    };

    /* Window managment methods */

    /** Close dialog window */
    void _closeWindow (void);
    /** Open Load prompt */
    void _openPromptWindowLoad (void);
    /** Open Save prompt */
    void _openPromptWindowSave (void);

    /* Event Handlers */

    /** Handler for CEGUI::Window::EventCloseClicked event.
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool _handleClose (const CEGUI::EventArgs&);
    /** Handler for Cancel button pushed event. */
    bool _handleCancel ();
    /** Handler for */
    void _okExecute (void);
    /** Handler for OK button pushed event. */
    bool _handleOk ();
    /** Handler for Back button pushed event. */
    bool _handleBack ();
    /** Handler for Double click on directory entry in file list. */
    bool _handleDirectorySelect ();
    /** Handler for Single click on file entry in file list. */
    bool _handleFileSelect ();
    /** Handler for Filter select in Filter combobox. */
    bool _handleFilterSelect ();
    /** Handler for Drive select in Drive combobox. */
    bool _handleDriveSelect ();
    /** Handler for edit in file editbox */
    void _handleFileInput ();
    /** Handler for edit in path editbox */
    void _handlePathInput ();
    /** Handler for OK event from Save prompt */
    bool _handlePromptSaveOk ();

    /* Other private methods */

    /** Determines whether a file exists in the selected path
     *
     * @param[in] szFileName - file name to check
     */
    bool _isFileExisting (const std::string szFileName);
    /** Fills the output object with generic data and clear the input data */
    void _prefillOutputAndClearInput (void);
    /** Determine all active drives */
    void _determineDrives (void);
    /** Get the current working directory (full qualified) */
    std::string _getWorkingPath (void);
    /** Set current drive
     * @param[in] drive - new drive to set, must be one of defined drives.
     */
    void _setCurrentDrive (const std::string& drive);
    /** Get the current drive.
     * This can be:
     * - The selected drive
     * - If no drive selected, the drive of the working directory is returned
     */
    std::string _getCurrentDrive (void);
    /** Get the current filter element. This can be:
     * - The selected one from the extensions combobox
     * - If no one selected, the first one from the list is returned
     */
    _Filter* _getCurrentFilterElement (void);
    /** Set a filename in the files editbox.
     * @param[in] szNewFileName - new file name to set
     */
    void _setCurrentFile (const std::string& szNewFileName);
    /** Get the filename from the file editbox. */
    std::string _getCurrentFile (void);
    /** Changes the directory of the selected drive.
     * For file systems with global root changes also selected drive to match
     * current directory. In case new directory is not a valid path no change
     * is performed.
     * @param[in] szDirectoryChange - new directory can be both relative and
     *            absolute
     */
    void _adjustPathOfSelectedDrive (const std::string& szDirectoryChange);
    /** Returns the current path of the selected drive. */
    _Path* _getPathOfSelectedDrive (void);
    /** Strip the left and right bracket of a string. */
    std::string _stripBrackets(const std::string& szItem);
    /** Returns the drive of a full qualified path.
     * For Windows this will be "C:" or something simillar. For *nix systems
     * return longest drive/bookmark path that matches beginning of the path.
     * If none of defined drives match returns empty string.
     */
    std::string _stripDrive(const std::string& szItem);
    /** Strips the extension of a file.
     * @param[in] szItem - file name to strip
     * @param[in] returnRightPart - if true return the file extension
     * @return file extension if returnRightPart was set to true, otherwise
     *         returns the filename without extension.
     */
    std::string _stripFileExtension(
	    const std::string& szItem, bool returnRightPart = true
	    );
    /** Determines whether an item (directory or filename) matches the selected
     * filter element.
     * @param[in] szItem - item to match.
     */
    bool _matchItem (const std::string& szItem);
    /** Fill the listbox with files. */
    void _fillListbox (void);

    /* Widgets */

    //! Main dialog window
    CEGUI::Window* _mWindow;
    //! Editbox, containing the file to be selected
    ScrollEditbox* _mEdtFile;
    //! Editbox, containing current path
    ScrollEditbox* _mEdtPath;
    //! Combobox, containing all active drives
    ScrollCombobox* _mCmbDrives;
    //! Combobox, that contains the filter elements (extensions)
    ScrollCombobox* _mCmbExtensions;
    //! Listbox, that represents the files and (sub)directories
    ScrollListbox* _mLbxFiles;
    // Message for the save prompt
    std::string _mStPromptSave;
    // Message for the load prompt
    std::string _mStPromptLoad;

    /* Other private attributes */

    //! Vector containing all drives
    std::vector<std::string> _drives;
    //! Map that contains the last selected dir's for each particular drive
    std::map<std::string, _Path*> _currentPaths;
    //! Object that contains the parsed filter
    _FilterParser _filterParser;

};
#endif // __CEGUIFILEDIALOG_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
