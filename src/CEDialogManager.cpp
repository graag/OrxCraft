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
 * @file CEDialogManager.cpp
 * @date 2012-07-01
 * @author fritz@fritzmahnke.com
 *
 * CEGUI implementation of the Dialog Manager.
 *
 * @todo Make sure dialogs get destroyed on program quit
 */

#include "CEDialogManager.h"

#include <string>

#include "CEGUI.h"

#include "ScrollFrameWindow.h"

// Dialogs
#include "ObjectEditor.h"
#include "FXSlotEditorWindow.h"
#include "ListPopup.h"

#include "CEGUICombobox.h"
#include "CEGUIEditbox.h"
#include "CEGUIListbox.h"
#include "CEGUICheckbox.h"
#include "CEGUIPushButton.h"

using std::string;
using std::make_pair;

CEDialogManager::~CEDialogManager ()
{
    DialogMapIterator iter;

    // Iterate through DialogMap and destroy both CEGUI::Windows and dialogs
    for (iter = m_dialogList.begin (); iter != m_dialogList.end (); ++iter)
    {
	CEGUI::WindowManager::getSingleton().destroyWindow(
		iter->second->GetWindowName());
	delete iter->second;
    }
    m_dialogList.clear();
};

DialogManager* CEDialogManager::GetInstance()
{
    if (!m_instance)
    {
	m_instance = new CEDialogManager();
    }

    return m_instance;
}

ScrollFrameWindow* CEDialogManager::MakeDialog (const string& dialogName,
	const string& dialogOptions)
{
    CEGUI::Window* window = orxNULL;
    ScrollFrameWindow *dialog = orxNULL;
    CEGUI::Window *windowRoot = orxNULL;

    /*
     * Check if dialog identified by name and options does not already exist.
     * If so activate it's window.
     */
    dialog = GetDialog(dialogName, dialogOptions);
    if(dialog != NULL) {
	orxASSERT(
		CEGUI::WindowManager::getSingleton().isWindowPresent(
		    dialog->GetWindowName())
		);
	window = CEGUI::WindowManager::getSingleton().getWindow(
		dialog->GetWindowName());
	window->activate();
	return dialog;
    }

    if (dialogName == "ObjectEditor")
    {
	dialog = new ObjectEditor (dialogName);
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "ObjectEditor.layout");
    }
    else if (dialogName == "FXSlotEditor")
    {
	dialog = new FXSlotEditorWindow (dialogName);
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "FXSlotEditor.layout");
    }
    else if (dialogName == "ListPopup")
    {
	dialog = new ListPopup (dialogName, dialogOptions);
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "ListPopup.layout");
    }
    else
    {
	orxASSERT (false);
    }

    CEGUI::Window *rootWindow = CEGUI::System::getSingleton ().getGUISheet ();
    window = windowRoot->getChildAtIdx (0);
    rootWindow->addChildWindow (window);

    int counter = window->getChildCount ();
    for (int i = 0; i < counter; i++)
    {
	CEGUI::Window* widget = window->getChildAtIdx (i);
	const orxSTRING type = widget->getType ().c_str ();
	if (orxString_Compare (type, "TaharezLook/TabControl") == 0)
	{
	    //TabControl
	    int c_counter = widget->getChildCount ();
	    for (int j = 0; j < c_counter; j++) {
		// Container - Pane, ScrollBar and other stuff
		CEGUI::Window* container = widget->getChildAtIdx (j);
		int t_counter = container->getChildCount ();
		for(int k = 0; k < t_counter; k++) {
		    // The actual Tab
		    CEGUI::Window* tab = container->getChildAtIdx(k);
		    const orxSTRING type = tab->getType ().c_str ();
		    // Tabs are of type "DefaultWindow"
		    if (orxString_Compare (type, "DefaultWindow") != 0)
			continue;
		    int w_counter = tab->getChildCount ();
		    for(int l = 0; l < w_counter; l++) {
			// Widget inside a Tab
			CEGUI::Window* sub_widget = tab->getChildAtIdx(l);
			LinkWidgetToDialog(sub_widget, dialog);
		    }
		}
	    }
	} else {
	    LinkWidgetToDialog(widget, dialog);
	}
    }

    dialog->Init (dialogName);
    window->activate();
    m_dialogList[dialog->GetId()] = dialog;

    return dialog;
}

void CEDialogManager::LinkWidgetToDialog(CEGUI::Window* widget, ScrollFrameWindow* dialog)
{
    const orxSTRING type = widget->getType ().c_str ();
    const string name = widget->getName().c_str();

    if (orxString_Compare (type, "TaharezLook/Checkbox") == 0)
    {
	CEGUICheckbox *checkbox = new CEGUICheckbox (dialog);
	checkbox->Init (name);
	dialog->AddWidget (checkbox);
    }
    else if (orxString_Compare (type, "TaharezLook/Combobox") == 0)
    {
	CEGUICombobox *combobox = new CEGUICombobox (dialog);
	combobox->Init(name);
	dialog->AddWidget (combobox);
    }
    else if (orxString_Compare (type, "TaharezLook/Editbox") == 0)
    {
	CEGUIEditbox *editbox = new CEGUIEditbox (dialog);
	editbox->Init(name);
	dialog->AddWidget (editbox);
    }
    else if (orxString_Compare (type, "TaharezLook/Listbox") == 0)
    {
	CEGUIListbox *listbox = new CEGUIListbox (dialog);
	listbox->Init(name);
	dialog->AddWidget (listbox);
    }
    else if (orxString_Compare (type, "TaharezLook/Button") == 0)
    {
	CEGUIPushButton *pushbutton = new CEGUIPushButton (dialog);
	pushbutton->Init(name);
	dialog->AddWidget (pushbutton);
    }
}

void CEDialogManager::DestroyDialog(const string& dialogName,
	const string& dialogOptions)
{
    ScrollFrameWindow* dialog = GetDialog(dialogName, dialogOptions);

    orxASSERT(dialog != NULL);

    CEGUI::WindowManager::getSingleton().destroyWindow(
	    dialog->GetWindowName());
    m_dialogList.erase(dialog->GetId());
    delete dialog;
}

void CEDialogManager::DestroyDialog(unsigned int id)
{
    ScrollFrameWindow* dialog = GetDialog(id);

    orxASSERT(dialog != NULL);

    CEGUI::WindowManager::getSingleton().destroyWindow(
	    dialog->GetWindowName());
    m_dialogList.erase(dialog->GetId());
    delete dialog;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
