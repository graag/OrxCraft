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
 * @file CEGUIDialogManager.cpp
 * @date 2012-07-01
 * @author fritz@fritzmahnke.com
 *
 * CEGUI implementation of the Dialog Manager.
 *
 * @todo Make sure dialogs get destroyed on program quit
 */

#include "cegui/CEGUIDialogManager.h"

#include "limits.h"
#include <string>

#include "CEGUI.h"

#include "ScrollFrameWindow.h"

// Dialogs
#include "ObjectEditor.h"
#include "FXSlotEditorWindow.h"

#include "cegui/CEGUIListPopup.h"
#include "cegui/CEGUITreePopup.h"
#include "cegui/CEGUICombobox.h"
#include "cegui/CEGUIEditbox.h"
#include "cegui/CEGUIListbox.h"
#include "cegui/CEGUITreebox.h"
#include "cegui/CEGUICheckbox.h"
#include "cegui/CEGUIPushButton.h"

using std::string;
using std::make_pair;

CEGUIDialogManager::~CEGUIDialogManager ()
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

ScrollFrameWindow* CEGUIDialogManager::MakeDialog (const string& dialogName,
	const string& dialogOptions)
{
    // Number of characters that represent maximum value of unsigned int.
    static int num_size = log10(UINT_MAX)+2;

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

    /*
     * The windows will be created with unique prefixes based on unique dialog
     * IDs. This will prevent name clashes between several instances of one
     * dialog instanciated with different options.
     *
     * num_buf is a Buffer to store window unique ID in string form
     */
    char num_buf[num_size];

    if (dialogName == "ObjectEditor")
    {
	dialog = new ObjectEditor (dialogName);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%d_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "ObjectEditor.layout", num_buf);
    }
    else if (dialogName == "FXSlotEditor")
    {
	dialog = new FXSlotEditorWindow (dialogName);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%d_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "FXSlotEditor.layout", num_buf);
    }
    else if (dialogName == "ListPopup")
    {
	dialog = new CEGUIListPopup (dialogName, dialogOptions);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%d_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "ListPopup.layout", num_buf);
    }
    else if (dialogName == "TreePopup")
    {
	dialog = new CEGUITreePopup (dialogName, dialogOptions);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%d_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "TreePopup.layout", num_buf);
    }
    else
    {
	orxASSERT (false);
    }

    // Get pointers to the global root window and the dialog window
    CEGUI::Window *rootWindow = CEGUI::System::getSingleton ().getGUISheet ();
    window = windowRoot->getChildAtIdx (0);
    // Transfer dialog ownership to the global root window.
    rootWindow->addChildWindow (window);
    // The temporary root defined in dialog layout is not needed anymore.
    CEGUI::WindowManager::getSingleton().destroyWindow(windowRoot);
    dialog->SetWindowName(window->getName().c_str());

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

    dialog->Init();
    window->activate();
    m_dialogList[dialog->GetId()] = dialog;

    return dialog;
}

void CEGUIDialogManager::LinkWidgetToDialog(CEGUI::Window* widget, ScrollFrameWindow* dialog)
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
    else if (orxString_Compare (type, "TaharezLook/Tree") == 0)
    {
	CEGUITreebox *treebox = new CEGUITreebox (dialog);
	treebox->Init(name);
	dialog->AddWidget (treebox);
    }
    else if (orxString_Compare (type, "TaharezLook/Button") == 0)
    {
	CEGUIPushButton *pushbutton = new CEGUIPushButton (dialog);
	pushbutton->Init(name);
	dialog->AddWidget (pushbutton);
    }
}

void CEGUIDialogManager::DestroyDialog(const string& dialogName,
	const string& dialogOptions)
{
    // Search for the dialog in the list of dialogs controlled by the manager.
    ScrollFrameWindow* dialog = GetDialog(dialogName, dialogOptions);

    orxASSERT(dialog != NULL);

    // Destroy the CEGUI window
    CEGUI::WindowManager::getSingleton().destroyWindow(
	    dialog->GetWindowName());
    // Remove the dialog from the list and destroy it
    m_dialogList.erase(dialog->GetId());
    delete dialog;
}

void CEGUIDialogManager::DestroyDialog(unsigned int id)
{
    // Search for the dialog in the list of dialogs controlled by the manager.
    ScrollFrameWindow* dialog = GetDialog(id);

    orxASSERT(dialog != NULL);

    // Destroy the CEGUI window
    CEGUI::WindowManager::getSingleton().destroyWindow(
	    dialog->GetWindowName());
    // Remove the dialog from the list and destroy it
    m_dialogList.erase(dialog->GetId());
    delete dialog;
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
