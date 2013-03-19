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
 * @file DialogManager.h
 * @date 2012-05-11
 * @author fritz@fritzmahnke.com
 *
 * The Dialog Manager is responsible for creating and destroying the program's
 * various user interfaces and their widgets (controls). The DialogManager base
 * class is the interface inherited by concrete implementations.
 */
#ifndef DIALOGMANAGER_H_
#define DIALOGMANAGER_H_

#include <string>
#include <map>

#include "ScrollFrameWindow.h"

//! DialogMap - type used for dialog window storage by the DialogManager
typedef std::map<unsigned int, ScrollFrameWindow*> DialogMap;
//! DialogMap iterator
typedef std::map<unsigned int, ScrollFrameWindow*>::iterator DialogMapIterator;
//! DialogMap const iterator
typedef std::map<unsigned int,
	ScrollFrameWindow*>::const_iterator DialogMapConstIterator;

class DialogManager
{
public:
    /** Create new dialog window.
     * @param[in] dialogName - name of the new dialog specifies predefined
     *                         dialog type
     * @param[in] dialogTitle - title of the dialog (optional)
     */
    virtual ScrollFrameWindow* MakeDialog (const std::string& dialogName,
	    const std::string& dialogTitle = "") = 0;
    /** Destroy a dialog window managed by the DialogManager
     * @param[in] id - unique ID of the dialog window
     */
    virtual void DestroyDialog(unsigned int id) = 0;
    /** Destroy a dialog window managed by the DialogManager,
     * identified by its name and options.
     * @param[in] dialogName - name of the dialog
     * @param[in] dialogOptions - options of the dialog
     */
    virtual void DestroyDialog(const std::string& dialogName,
	    const std::string& dialogOptions = "") = 0;
    /** Get a dialog window managed by the DialogManager
     * @param[in] id - unique ID of the dialog window
     * @return a pointer to the dialog if found, NULL otherwise
     */
    virtual ScrollFrameWindow* GetDialog(unsigned int id);
    /** Get a dialog window managed by the DialogManager,
     * identified by its name and options.
     * @param[in] dialogName - name of the dialog
     * @param[in] dialogOptions - options of the dialog
     * @return a pointer to the dialog if found, NULL otherwise
     */
    virtual ScrollFrameWindow* GetDialog(const std::string& dialogName,
	    const std::string& dialogOptions = "");

    void OnReset();

    //! D-tor
    virtual ~DialogManager ()
    {
	DialogMapIterator iter;
	for (iter = m_dialogList.begin ();
	    iter != m_dialogList.end ();
	    ++iter)
	{
	    delete iter->second;
	}
	m_dialogList.clear();
    };

protected:
    // Pointer to an instance of the DialogManager singleton
    static DialogManager* m_instance;

    // Dialogs managed by the manager
    DialogMap m_dialogList;
};

#endif	// DIALOGMANAGER_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
