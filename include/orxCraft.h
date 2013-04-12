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
 * @file OrxCraft.h
 * @date 2012-05-04
 * @author fritz@fritzmahnke.com
 *
 */
#ifndef ORXCRAFT_H
#define ORXCRAFT_H

//! Includes

/*
 *  Uncomment above define to prevent the embedded editor (ScrollEd) from
 *  being compiled
 */
#include <vector>
#include <string>

#include "Scroll.h"

class DialogManager;

class ScrollGUICEGUI;

using std::vector;
using std::string;

//! Main OrxCraft program class
class OrxCraft : public Scroll<OrxCraft>
{
public:
    OrxCraft ();

    //! Get names of currently loaded objects
    inline std::vector<std::string>& GetObjectList () { return m_objectList; }
    std::vector<std::string> GetObjectListSafe(const std::string& name);
    //! Get names of currently loaded FXs
    inline std::vector<std::string>& GetFXList () { return m_fxList; }
    //! Get names of currently loaded FXSlots
    inline std::vector<std::string>& GetFXSlotList () { return m_fxSlotList; }

    //! Get names of currently loaded Graphics
    inline std::vector<std::string>& GetGraphicList () { return m_graphicList; }

    /*!
     * Get an Scroll bound object with specified name.
     *
     * @return an object if found, orxNULL otherwise.
     */
    ScrollObject * GetObjectByName (const std::string& name) const;
    DialogManager * GetDialogManager () { return m_dialogManager; }

    //! Setup loaded config section lists and create objects.
    void SetupConfig ();
    //! Save the orxCraft user configuration to a file.
    orxSTATUS SaveEditorConfig () const;
    //! Save the currently open project to disk.
    orxSTATUS SaveProject () const;

    //! Indicate properties have been changed and objects need to be updated
    //! to reflect that change.
    void NeedObjectUpdate ();

private:
    //! Initialize the editor: parse command line parameters, load user
    //! settings, load objects, create GUI.
    virtual orxSTATUS Init ();
    //! Fast loop.
    virtual orxSTATUS Run ();
    //! Called on exit to clean up.
    virtual void      Exit ();
    //! Binds orx objects to their ScrollObjects.
    virtual void      BindObjects ();
    //! Main loop executed on main clock ticks.
    virtual void      Update (const orxCLOCK_INFO &_rstInfo);

    //! Load config file for editing.
    void InitConfig ();
    //! Load from config user settings.
    void LoadUserSettings ();

    //! Mouse button push callback.
    void OnMouseDown ();
    //! Mouse button release callback.
    void OnMouseUp ();
    //! Key press callback.
    void OnKeyPress (const orxSTRING key);
    //! Key release callback.
    void OnKeyRelease (const orxSTRING key);
    void KeyRepeat();

    //! Auto save current project state to a project.ini.swp files.
    orxSTATUS SaveBackup() const;
    //! @todo AddActionDisplay should be in a GUI class
    //! Display a visual notification of an action on screen.
    //! @param[in] action String to display
    orxSTATUS AddActionDisplay (const orxSTRING action) const;

    //! Handler for orx events.
    static orxSTATUS orxFASTCALL EventHandler (const orxEVENT *_pstEvent);
    //! Filter out user settings to save.
    static orxBOOL orxFASTCALL SaveConfigFilter	(const orxSTRING sectionName,
						 const orxSTRING keyName,
						 const orxSTRING fileName,
						 orxBOOL useEncryption);
    //! Filter out project contents to save.
    static orxBOOL orxFASTCALL SaveProjectFilter (const orxSTRING sectionName,
						  const orxSTRING keyName,
						  const orxSTRING fileName,
						  orxBOOL useEncryption);
    //! Process command line parameters.
    static orxSTATUS orxFASTCALL ProcessParams(orxU32 paramCount,
					       const orxSTRING params[]);

    //! Windowed part of the GUI.
    ScrollGUICEGUI	    *m_gui;
    //! Dialog factory object.
    DialogManager	    *m_dialogManager;

    /// Currently loaded config objects
    std::vector<std::string> m_objectList;
    /// Currently loaded config graphics
    std::vector<std::string> m_graphicList;
    /// Currently loaded FXs
    std::vector<std::string> m_fxList;
    /// Currently loaded FXSlots
    std::vector<std::string> m_fxSlotList;

    //! Modified project flag: reload object.
    bool m_dirty;
    //! Modified project flag: requires save upon exit.
    bool m_dirtySave;
    //! Modified project flag: used for auto save suppresion.
    bool m_dirtyAutosave;
    //! Editor execution time.
    orxFLOAT m_localTime;
    //! Last auto save time stamp.
    orxFLOAT m_autoSaveTimeStamp;
    //! Interval used for auot saves.
    orxFLOAT m_autoSaveInterval;
    //! Name of currently open project file
    static std::string m_projectFileName;
};

#endif // ORXCRAFT_H

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
