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
 * @file CEGUIGui.cpp
 * @date 2012-06-08
 * @author fritz@fritzmahnke.com
 *
 */
#include "cegui/CEGUIGui.h"

#include <string>

#include "orxCraft.h"
#include "ObjectEditor.h"
#include "FXSlotEditorWindow.h"
#include "InfoWindow.h"

#include "cegui/CEGUIExtendedTree.h"

#include "constants.h"

#include "utf8-cpp/utf8.h"

using CEGUI::System;
using std::string;

CEGUIGui::CEGUIGui () :
    m_glRenderer (orxNULL)
{
}

CEGUIGui::~CEGUIGui ()
{
    //m_glRenderer->destroySystem ();
    m_glRenderer = orxNULL;
}

void CEGUIGui::Init ()
{
    // CEGUI renderer has to be initialized first
    m_glRenderer = & CEGUI::OpenGLRenderer::bootstrapSystem();

    // Scroll object initializes CEGUI on creation
    OrxCraft::GetInstance ().CreateObject (scrollGUIName);
}

void CEGUIGui::CEGUIScrollObject::OnCreate ()
{
    using namespace CEGUI;

    // Initialise the required dirs for the DefaultResourceProvider
    // All CEGUI data files are stored in data/cegui subdirectory relative to OrxCraft executable
    DefaultResourceProvider* rp =
	static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", "./cegui/schemes/");
    rp->setResourceGroupDirectory("imagesets", "./cegui/imagesets/");
    rp->setResourceGroupDirectory("fonts", "./cegui/fonts/");
    rp->setResourceGroupDirectory("layouts", "./cegui/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "./cegui/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", "./cegui/lua_scripts/");
    rp->setResourceGroupDirectory("schemas", "./cegui/xml_schemas/");
    // Set the default resource groups to be used
    Imageset::setDefaultResourceGroup("imagesets");
    Font::setDefaultResourceGroup("fonts");
    Scheme::setDefaultResourceGroup("schemes");
    WidgetLookManager::setDefaultResourceGroup("looknfeels");
    WindowManager::setDefaultResourceGroup("layouts");
    ScriptModule::setDefaultResourceGroup("lua_scripts");
    // Setup default group for validation schemas
    XMLParser* parser = System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
	parser->setProperty("SchemaDefaultResourceGroup", "schemas");

    SchemeManager::getSingleton().create("TaharezLook.scheme");
    // Here we can load additional fonts
    //FontManager::getSingleton().create("DejaVuSans-10.font");

    // Register custom widgets
    CEGUI::WindowFactoryManager::getSingleton().addFactory
	<CEGUI::TplWindowFactory<CEGUIExtendedTree> >();
    // Make sure custom widget are used when met in a layout
    CEGUI::WindowFactoryManager::getSingleton().addWindowTypeAlias(
	    "CEGUI/Tree", "CEGUIExtendedTree");

    WindowManager &winManager = WindowManager::getSingleton ();
    Window* myRoot = winManager.loadWindowLayout ("Main.layout");
    System::getSingleton().setGUISheet( myRoot );
}

void CEGUIGui::CEGUIScrollObject::OnDelete ()
{
}

void CEGUIGui::InputMouseMove ()
{
    orxVECTOR mousePos;
    orxMouse_GetPosition (&mousePos);

    System::getSingleton ().injectMousePosition (mousePos.fX, mousePos.fY);
}

void CEGUIGui::InputMouseDown ()
{
    orxVECTOR mousePos;
    orxMouse_GetPosition (&mousePos);

    System::getSingleton ().injectMousePosition (mousePos.fX, mousePos.fY);
    System::getSingleton ().injectMouseButtonDown (CEGUI::LeftButton);
}

void CEGUIGui::InputMouseUp ()
{
    orxVECTOR mousePos;
    orxMouse_GetPosition (&mousePos);

    System::getSingleton ().injectMousePosition (mousePos.fX, mousePos.fY);
    System::getSingleton ().injectMouseButtonUp (CEGUI::LeftButton);
}

void CEGUIGui::InputKeyPress (const orxSTRING orxKey)
{
    using CEGUI::Key;
    int key = 0;

    if (orxString_Compare (orxKey, "LeftArrow") == 0)
    {
	key = Key::ArrowLeft;
    }
    if (orxString_Compare (orxKey, "RightArrow") == 0)
    {
	key = Key::ArrowRight;
    }
    if (orxString_Compare (orxKey, "UpArrow") == 0)
    {
	key = Key::ArrowUp;
    }
    if (orxString_Compare (orxKey, "DownArrow") == 0)
    {
	key = Key::ArrowDown;
    }
    if (orxString_Compare (orxKey, "Home") == 0)
    {
	key = Key::Home;
    }
    if (orxString_Compare (orxKey, "End") == 0)
    {
	key = Key::End;
    }
    if (orxString_Compare (orxKey, "Delete") == 0)
    {
	key = Key::Delete;
    }
    if (orxString_Compare (orxKey, "Backspace") == 0)
    {
	key = Key::Backspace;
    }
    if (orxString_Compare (orxKey, "Return") == 0)
    {
	key = Key::Return;
    }
    if (orxString_Compare (orxKey, "LeftCtrl") == 0)
    {
	key = Key::LeftControl;
    }
    if (orxString_Compare (orxKey, "LeftShift") == 0)
    {
	key = Key::LeftShift;
    }

    System::getSingleton ().injectKeyDown (key);
}

void CEGUIGui::InputKeyRelease (const orxSTRING orxKey)
{
    using CEGUI::Key;
    int key = 0;

    if (orxString_Compare (orxKey, "LeftCtrl") == 0)
    {
	key = Key::LeftControl;
    }
    if (orxString_Compare (orxKey, "LeftShift") == 0)
    {
	key = Key::LeftShift;
    }

    System::getSingleton ().injectKeyUp (key);
}

void CEGUIGui::InputString (const string& inputString)
{
    using utf8::utf8to32;

    /**
     * CEGUI::System::injectChar expects utf32 encoding. Convert utf8 used by
     * orx thanks to utf8-cpp library
     */
    vector<int> utf32result;
    utf8to32(inputString.begin(),  inputString.end(), back_inserter(utf32result));

    vector<int>::iterator iter;
    for(iter = utf32result.begin(); iter != utf32result.end(); iter++) {
	System::getSingleton().injectChar(*iter);
    }
}

void CEGUIGui::Update (const orxCLOCK_INFO &_rstInfo)
{
    orxVECTOR mousePos;
    orxMouse_GetPosition (&mousePos);

    orxVECTOR worldPos;
    orxRender_GetWorldPosition (&mousePos, &worldPos);

    // GUI windows are on top of Orx objects. Check if mouse is inside of a window.
    CEGUI::System::getSingleton ().injectMousePosition (mousePos.fX,
							mousePos.fY);
    CEGUI::Window *window =
	CEGUI::System::getSingleton ().getWindowContainingMouse ();
    // Root window covers whole viewport but it is invisible.
    if (window != NULL &&
	orxString_Compare (window->getName().c_str(), "root") != 0)
    {
	// Pass input to GUI
	InputMouseMove();
    }
}

orxBOOL CEGUIGui::CEGUIScrollObject::OnRender ()
{
    DrawGrid ();
    System::getSingleton().renderGUI();
    return false; 
}

void CEGUIGui::CEGUIScrollObject::DrawGrid ()
{
    orxConfig_PushSection ("MainCamera");
    float frustumWidth = orxConfig_GetFloat ("FrustumWidth");
    float frustumHeight = orxConfig_GetFloat ("FrustumHeight");
    orxConfig_PopSection ();

    int gridRes = 100;
    int columns = (int) frustumWidth / gridRes;
    int rows = (int) frustumHeight / gridRes;

    orxRGBA gridColor = orx2RGBA (200, 0, 0, 255);

    for (int i = 1; i <= columns; i++)
    {
	orxVECTOR start = { {(float) i * gridRes}, {0}, {0}};
	orxVECTOR end   = { {(float) i * gridRes}, {frustumHeight}, {0}};
	orxDisplay_DrawLine (&start, &end, gridColor);
    }

    for (int i = 1; i <= rows; i++)
    {
	orxVECTOR start = {{0}, {(float) i * gridRes}, {0}};
	orxVECTOR end   = {{frustumWidth}, {(float) i * gridRes}, {0}};
	orxDisplay_DrawLine (&start, &end, gridColor);
    }
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
