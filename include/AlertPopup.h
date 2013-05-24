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
 * @file AlertPopup.h
 * @date 2013-04-26
 * @author graag@o2.pl
 *
 */
#ifndef __ALERTPOPUP_H__
#define __ALERTPOPUP_H__

#include <string>

#include "ScrollFrameWindow.h"

/** Class defining interface for popup alert window.
 *
 * Alert window displays a message with an "OK" button.
 */
class AlertPopup : public ScrollFrameWindow
{
public:
    /** C-tor */
    AlertPopup(const std::string& name, const std::string& title = "");

    /** Set the text displayed in the popup */
    virtual void SetText(const std::string& message) = 0;

    /* ScrollFrameWindow interface */

    virtual void OnAction(const std::string& widgetName,
	    const std::string& action = "");
    virtual void OnInput(const std::string& widgetName);
    virtual void OnClose();
    virtual void OnReset() { OnClose(); }
    virtual void HandlePopup(const std::string& popupName, orxU32 popupID);
    virtual void HandleClose(const std::string& popupName, orxU32 popupID);

    /* Signals */

    /** Notify caller on popup finish. */
    Gallant::Signal2<const std::string&, orxU32> SignalFinish;
};

#endif	// __ALERTPOPUP_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
