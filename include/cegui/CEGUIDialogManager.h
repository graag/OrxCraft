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
 * @file CEGUIDialogManager.h
 * @date 2012-07-01
 * @author fritz@fritzmahnke.com
 *
 */

#ifndef CEGUIDIALOGMANAGER_H_
#define CEGUIDIALOGMANAGER_H_

#include <string>
#include <stack>

#include "DialogManager.h"
#include "ScrollFrameWindow.h"

#include "CEGUI.h"

class CEGUIDialogManager : public DialogManager
{
public:
    virtual ScrollFrameWindow* OpenDialog (const std::string& dialogName,
	    const std::string& dialogOptions = "");
    virtual void DestroyDialog(unsigned int id);
    virtual void DestroyDialog(const std::string& dialogName);
    virtual void CloseDialog(unsigned int id);
    virtual void CloseDialog(const std::string& dialogName);

    virtual ~CEGUIDialogManager ();

private:
    /** Assign a widget to a dialog.
     * @param[in] widget - a pointer to a CEGUI widget
     * @param[in] dialog - a pointer to a dialog that the widget will be
     *                     assigned to
     */
    void LinkWidgetToDialog(CEGUI::Window* widget, ScrollFrameWindow* dialog);
    void SetModalState(bool state, CEGUI::Window* window);

    std::stack<CEGUI::Window*> m_modalStack;
};

#endif    // CEGUIDIALOGMANAGER_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
