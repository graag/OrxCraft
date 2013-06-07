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
#include "ProjectBrowser.h"

#include "cegui/CEGUIListPopup.h"
#include "cegui/CEGUITreePopup.h"
#include "cegui/CEGUIFileDialog.h"
#include "cegui/CEGUIAlertPopup.h"
#include "cegui/CEGUIConfirmPopup.h"

#include "cegui/CEGUICombobox.h"
#include "cegui/CEGUIEditbox.h"
#include "cegui/CEGUIMultiLineEditbox.h"
#include "cegui/CEGUIListbox.h"
#include "cegui/CEGUITreebox.h"
#include "cegui/CEGUICheckbox.h"
#include "cegui/CEGUIPushButton.h"
#include "cegui/CEGUITextbox.h"

using std::string;
using std::make_pair;

using CEGUI::Window;

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

ScrollFrameWindow* CEGUIDialogManager::OpenDialog (const string& dialogName,
	const string& dialogTitle)
{
    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER, "OpenDialog: %s", dialogName.c_str());
    // Number of characters that represent maximum value of unsigned int.
    static int num_size = log10(UINT_MAX)+2;

    CEGUI::Window* window = orxNULL;
    ScrollFrameWindow *dialog = orxNULL;
    CEGUI::Window *windowRoot = orxNULL;

    /*
     * Check if dialog identified by name and options does not already exist.
     * If so activate it's window.
     */
    dialog = GetDialog(dialogName);
    if(dialog != NULL) {
	orxASSERT(
		CEGUI::WindowManager::getSingleton().isWindowPresent(
		    dialog->GetWindowName())
		);
	window = CEGUI::WindowManager::getSingleton().getWindow(
		dialog->GetWindowName());
	window->show();
	window->activate();
	if(dialog->IsModal()) {
	    SetModalState(true, window);
	}
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
	int result = snprintf(num_buf, num_size, "%lu_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "ObjectEditor.layout", num_buf);
    }
    else if (dialogName == "FXSlotEditor")
    {
	dialog = new FXSlotEditorWindow (dialogName);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%lu_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "FXSlotEditor.layout", num_buf);
    }
    else if (dialogName == "ProjectBrowser")
    {
	dialog = new ProjectBrowser(dialogName);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%lu_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "ProjectBrowser.layout", num_buf);
    }
    else if (dialogName == "ListPopup")
    {
	dialog = new CEGUIListPopup (dialogName, dialogTitle);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%lu_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "ListPopup.layout", num_buf);
    }
    else if (dialogName == "TreePopup")
    {
	dialog = new CEGUITreePopup (dialogName, dialogTitle);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%lu_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "TreePopup.layout", num_buf);
    }
    else if (dialogName == "FileDialog")
    {
	dialog = new CEGUIFileDialog (dialogName, dialogTitle);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%lu_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "FileDialog.layout", num_buf);
    }
    else if (dialogName == "AlertPopup")
    {
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER, "Create AlertPopup");
	dialog = new CEGUIAlertPopup (dialogName, dialogTitle);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%lu_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "AlertPopup.layout", num_buf);
    }
    else if (dialogName == "ConfirmPopup")
    {
	dialog = new CEGUIConfirmPopup (dialogName, dialogTitle);
	// Generate a unique prefix
	int result = snprintf(num_buf, num_size, "%lu_", dialog->GetId());
	orxASSERT(result < num_size); // Make sure snprintf succeeded
	windowRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout(
	    "ConfirmPopup.layout", num_buf);
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

    if(dialog->IsModal()) {
	SetModalState(true, window);
    }

    DialogMapConstIterator dialogIter = m_dialogList.begin();
    for(;dialogIter != m_dialogList.end(); dialogIter++) {
    	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER, "AfterDialog: %s", dialogIter->second->GetName().c_str());
    }

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
    else if (orxString_Compare (type, "TaharezLook/MultiLineEditbox") == 0)
    {
	CEGUIMultiLineEditbox *editbox = new CEGUIMultiLineEditbox (dialog);
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
    else if (orxString_Compare (type, "TaharezLook/StaticText") == 0)
    {
	/* We only consider text widgets for dynamic content. Such widgets
	 * should be named as "Dynamic<WidgetName>" as CEGUI does not
	 * distinguish between static or dynamic text widgets.
	 */
	if(name.find("Dynamic") != string::npos)
	{
		CEGUITextbox *textbox = new CEGUITextbox (dialog);
		textbox->Init(name);
		dialog->AddWidget (textbox);
	}
    }
}

void CEGUIDialogManager::DestroyDialog(const string& dialogName)
{
    // Search for the dialog in the list of dialogs controlled by the manager.
    ScrollFrameWindow* dialog = GetDialog(dialogName);

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

void CEGUIDialogManager::CloseDialog(const string& dialogName)
{
    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER, "CloseDialog: %s", dialogName.c_str());
    // Search for the dialog in the list of dialogs controlled by the manager.
    ScrollFrameWindow* dialog = GetDialog(dialogName);

    orxASSERT(dialog != NULL);

    // Hide the CEGUI window
    CEGUI::Window* window = CEGUI::WindowManager::getSingleton().getWindow(
	    dialog->GetWindowName());
    window->hide();
    if(dialog->IsModal())
	SetModalState(false, window);
}

void CEGUIDialogManager::CloseDialog(unsigned int id)
{
    // Search for the dialog in the list of dialogs controlled by the manager.
    ScrollFrameWindow* dialog = GetDialog(id);

    orxASSERT(dialog != NULL);

    // Hide the CEGUI window
    CEGUI::Window* window = CEGUI::WindowManager::getSingleton().getWindow(
	    dialog->GetWindowName());
    window->hide();
    if(dialog->IsModal())
	SetModalState(false, window);
}

void CEGUIDialogManager::SetModalState(bool state, Window* window)
{
    orxASSERT(window != NULL);

    // No change from current state
    // We do not assert as not to mess with unlikely case where modal window
    // created a modal popup and got killed while popup was open
    if(state == window->getModalState())
	return;

    window->setModalState(state);
    if(state)
    {
    	m_modalStack.push(window);
    }
    else
    {
	orxASSERT(!m_modalStack.empty())
	m_modalStack.pop();
	if(!m_modalStack.empty())
	{
	    Window* prev = m_modalStack.top();
	    prev->setModalState(true);
	}
    }
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
