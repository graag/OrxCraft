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
 * @file CEGUIConfirmPopup.h
 * @date 2013-04-26
 * @author graag@o2.pl
 *
 */
#ifndef __CEGUICONFIRMPOPUP_H__
#define __CEGUICONFIRMPOPUP_H__

#include <string>

#include "CEGUI.h"

#include "ConfirmPopup.h"

#include "ScrollTextbox.h"

class CEGUIConfirmPopup : public ConfirmPopup
{
public:
    /** C-tor */
    CEGUIConfirmPopup(const std::string& name, const std::string& title = "");

    /* ConfirmPopup interface */

    virtual void SetText(const std::string& message);

    /* ScrollFrameWindow interface */

    virtual void Init ();

private:
    /** Handler for CEGUI::Window::EventCloseClicked event.
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnCloseClicked (const CEGUI::EventArgs &e);

    ScrollTextbox* m_message;
};

#endif	// __CEGUICONFIRMPOPUP_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
