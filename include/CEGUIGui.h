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

#ifndef CEGUIGUI_H_
#define CEGUIGUI_H_
/**
 * @file CEGUIGui.h
 * @date 2012-06-08
 * @author fritz@fritzmahnke.com
 *
 * The CEGUI implementation of a Scroll GUI
 */

#include "Scroll.h"

#include "ScrollGui.h"

#include "CEGUI.h"
#include "RendererModules/OpenGL/CEGUIOpenGLRenderer.h"

#include <string>

class ScrollFrameWindow;

/**
 *  Renders GUI items and sends input from Scroll to them.
 */
class CEGUIGui : public ScrollGui
{
public:
    /** C-tor */
    CEGUIGui ();
    /** D-tor */
    ~CEGUIGui ();

    void Init ();
    void InputMouseMove  ();
    void InputMouseDown  ();
    void InputMouseUp    ();
    void InputKeyPress   (const orxSTRING orxKey);
    void InputKeyRelease (const orxSTRING orxKey);
    void InputString     (const std::string& inputString);
    void Update (const orxCLOCK_INFO &_rstInfo);

    class CEGUIScrollObject : public ScrollObject
    {
	virtual void    OnCreate ();
	virtual void    OnDelete ();
	virtual orxBOOL OnRender ();
	/** Calculate and render the editor grid */
	void DrawGrid ();
    };

private:

    CEGUI::OpenGLRenderer *m_glRenderer;
};

#endif  // CEGUIGUI_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
