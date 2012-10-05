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
 * @file CEDialogManager.h
 * @date 2012-07-01
 * @author fritz@fritzmahnke.com
 *
 */

#ifndef CEDIALOGMANAGER_H_
#define CEDIALOGMANAGER_H_

#include <string>

#include "DialogManager.h"
#include "ScrollFrameWindow.h"

#include "CEGUI.h"

class CEDialogManager : public DialogManager
{
public:
    /** Create new dialog window.
     * @param[in] dialogName - name of the new dialog specifies predefined
     *                         dialog type:
     *                         * ObjectEditor
     *                         * FXSlotEditor
     *                         * ListPopup
     * @param[in] dialogOptions - options passed to the new dialog (optional)
     */
    virtual ScrollFrameWindow* MakeDialog (const std::string& dialogName,
	    const std::string& dialogOptions = "");
    //! Return an instance of the DialogManager singleton.
    virtual DialogManager* GetInstance();
    /** Destroy a dialog window managed by the DialogManager.
     * Will destroy the corresponding CEGUI::Window.
     * @param[in] id - unique ID of the dialog window
     */
    virtual void DestroyDialog(unsigned int id);
    /** Destroy a dialog window managed by the DialogManager,
     * identified by its name and options.
     * Will destroy the corresponding CEGUI::Window.
     * @param[in] dialogName - name of the dialog
     * @param[in] dialogOptions - options of the dialog
     */
    virtual void DestroyDialog(const std::string& dialogName,
	    const std::string& dialogOptions = "");

    virtual ~CEDialogManager ();

private:
    /** Assign a widget to a dialog.
     * @param[in] widget - a pointer to a CEGUI widget
     * @param[in] dialog - a pointer to a dialog that the widget will be
     *                     assigned to
     */
    void LinkWidgetToDialog(CEGUI::Window* widget, ScrollFrameWindow* dialog);
};

#endif    // CEDIALOGMANAGER_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
