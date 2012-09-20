/**
 * @file OrxCraft.h
 * @date 2012-05-04
 * @author fritz@fritzmahnke.com
 *
 */
#ifndef ORXCRAFT_H
#define ORXCRAFT_H

//! Includes
// #define __NO_SCROLLED__
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
    //! Get names of currently loaded FXSlots
    inline std::vector<std::string>& GetFXSlotList () { return m_fxSlotList; }
    //! Set selected FXSlot in FXSlot Editor
    void SetSelectedFXSlot (const orxSTRING name);

    //! Get names of currently loaded Graphics
    inline std::vector<std::string>& GetGraphicList () { return m_graphicList; }

    ScrollObject * GetObjectByName (const orxSTRING name) const;
    DialogManager * GetDialogManager () { return m_dialogManager; }

    //! Setup loaded config section lists and create objects
    void SetupConfig ();
    //! Save the editor configuration to a file
    void SaveEditorConfig ();

    /** 
     *  Indicate properties have been changed and objects need to be updated
     *  to reflect that change.
     */
    void NeedObjectUpdate ();

private:
    virtual orxSTATUS Init ();
    virtual orxSTATUS Run ();
    virtual void      Exit ();
    virtual void      BindObjects ();
    virtual void      Update (const orxCLOCK_INFO &_rstInfo);

    /** Load config file for editing */
    void InitConfig ();

    void OnMouseDown ();
    void OnMouseUp ();
    void OnKeyPress (const orxSTRING key);
    static orxSTATUS orxFASTCALL EventHandler (const orxEVENT *_pstEvent);

    static orxBOOL orxFASTCALL SaveConfigFunction
	(const orxSTRING _zSectionName,
         const orxSTRING _zKeyName,
	 const orxSTRING _zFileName,
	 orxBOOL _bUseEncryption);

    ScrollGUICEGUI	    *m_gui;
    DialogManager	    *m_dialogManager;

    /// Currently loaded config objects
    std::vector<std::string> m_objectList;
    /// Currently loaded config graphics
    std::vector<std::string> m_graphicList;
    /// Currently loaded FXSlots
    std::vector<std::string> m_fxSlotList;

    bool m_dirty;
};

#endif // ORXCRAFT_H

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
