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

#ifndef SCROLLGUI_H_
#define SCROLLGUI_H_
/**
 * @file ScrollGUI.h
 * @date 2012-06-08
 * @author fritz@fritzmahnke.com
 *
 * The Scroll GUI
 */

#include "Scroll.h"

#include <string>

/**
 *  Renders GUI items and sends input from Scroll to them.
 */
class ScrollGui
{
public:
    virtual void Init () = 0;
    virtual void InputMouseMove  () = 0;
    virtual void InputMouseDown  () = 0;
    virtual void InputMouseUp    () = 0;
    virtual void InputKeyPress   (const orxSTRING orxKey) = 0;
    virtual void InputKeyRelease (const orxSTRING orxKey) = 0;
    virtual void InputString     (const std::string& inputString) = 0;
    virtual void Update (const orxCLOCK_INFO &_rstInfo) = 0;
};

#endif  // SCROLLGUI_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
