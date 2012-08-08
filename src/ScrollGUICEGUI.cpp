/**
 * @file ScrollGUICEGUI.cpp
 * @date 2012-06-08
 * @author fritz@fritzmahnke.com
 *
 */
#include "ScrollGUICEGUI.h"
#include "OrxCraft.h"

ScrollGUICEGUI::ScrollGUICEGUI () :
    m_glRenderer (orxNULL)
{
}

void ScrollGUICEGUI::OnCreate ()
{
    m_glRenderer = & CEGUI::OpenGLRenderer::bootstrapSystem ();
    // Initialise the required dirs for the DefaultResourceProvider
    // All CEGUI data files are stored in data/cegui subdirectory relative to OrxCraft executable
    CEGUI::DefaultResourceProvider* rp = 
	static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", "./cegui/schemes/");
    rp->setResourceGroupDirectory("imagesets", "./cegui/imagesets/");
    rp->setResourceGroupDirectory("fonts", "./cegui/fonts/");
    rp->setResourceGroupDirectory("layouts", "./cegui/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "./cegui/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", "./cegui/lua_scripts/");
    rp->setResourceGroupDirectory("schemas", "./cegui/xml_schemas/");
    // Set the default resource groups to be used
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
    // Setup default group for validation schemas
    CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
	parser->setProperty("SchemaDefaultResourceGroup", "schemas");
    // Load OrxCraft GUI settings
    CEGUI::SchemeManager::getSingleton ().create ("TaharezLook.scheme");
    CEGUI::Window* myRoot =
	CEGUI::WindowManager::getSingleton ().loadWindowLayout ("Main.layout");
    CEGUI::System::getSingleton ().setGUISheet (myRoot);

    // Creates spritesheet viewport
    m_sheetViewport = orxViewport_CreateFromConfig("SpriteSheetViewport");
    // Gets spritesheet camera
    m_sheetCamera = orxViewport_GetCamera(m_sheetViewport);
    // Create scrollbars
    m_sheetBarX = orxObject_CreateFromConfig("SheetBarX");
    m_sheetBarY = orxObject_CreateFromConfig("SheetBarY");
    m_sheetBarBgX = orxObject_CreateFromConfig("SheetBarBgX");
    m_sheetBarBgY = orxObject_CreateFromConfig("SheetBarBgY");
    m_objectBarX = orxObject_CreateFromConfig("ObjectBarX");
    m_objectBarY = orxObject_CreateFromConfig("ObjectBarY");
    m_objectBarBgX = orxObject_CreateFromConfig("ObjectBarBgX");
    m_objectBarBgY = orxObject_CreateFromConfig("ObjectBarBgY");
    m_sheetZoom = 1.0;
    m_objectZoom = 1.0;

    UpdateScrollBars();
}

void ScrollGUICEGUI::OnDelete ()
{
    //m_glRenderer->destroySystem ();
    m_glRenderer = orxNULL;
}

void ScrollGUICEGUI::InputMouseMove ()
{
    orxVECTOR mousePos;
    orxMouse_GetPosition (&mousePos);

    CEGUI::System::getSingleton ().injectMousePosition (mousePos.fX,
							mousePos.fY);
}

void ScrollGUICEGUI::InputMouseDown ()
{
    orxVECTOR mousePos;
    orxMouse_GetPosition (&mousePos);
    orxVECTOR worldPos;
    orxRender_GetWorldPosition (&mousePos, &worldPos);
    orxFLOAT step = 10;

    CEGUI::System::getSingleton ().injectMousePosition (mousePos.fX, mousePos.fY);
    CEGUI::Window *window = CEGUI::System::getSingleton ().getWindowContainingMouse ();

    if(window != NULL && orxString_Compare(window->getName().c_str(), "root") != 0) {
	CEGUI::System::getSingleton ().injectMouseButtonDown (CEGUI::LeftButton);
    } else {
	// Make sure that we pick objects for all viewports (actually main
	// starts at -1)
	worldPos.fZ=-10;

	// Picks object
	orxOBJECT *pickedObject = orxObject_Pick(&worldPos);
	// Found?
	if(pickedObject) {
	    orxVECTOR cameraPos;
	    orxFLOAT sign;
	    if(orxInput_IsActive("LeftClick") ) {
		sign = -1.0;
	    } else if (orxInput_IsActive("RightClick")) {
		sign =  1.0;
	    }
	    // Scrollbars
	    if (orxString_Compare (
			orxObject_GetName(pickedObject), "SheetBarX") == 0 ||
		    orxString_Compare(
			orxObject_GetName(pickedObject), "SheetBarBgX") == 0 ) {
		orxCamera_GetPosition(m_sheetCamera, &cameraPos);
		cameraPos.fX = cameraPos.fX + sign*step;
		orxCamera_SetPosition(m_sheetCamera, &cameraPos);
	    } else if (orxString_Compare (
			orxObject_GetName(pickedObject), "SheetBarY") == 0 ||
		    orxString_Compare(
			orxObject_GetName(pickedObject), "SheetBarBgY") == 0 ) {
		orxCamera_GetPosition(m_sheetCamera, &cameraPos);
		cameraPos.fY = cameraPos.fY + sign*step;
		orxCamera_SetPosition(m_sheetCamera, &cameraPos);
	    } else if (orxString_Compare (
			orxObject_GetName(pickedObject), "ObjectBarX") == 0 ||
		    orxString_Compare(
			orxObject_GetName(pickedObject), "ObjectBarBgX") == 0 ) {
		orxCamera_GetPosition(OrxCraft::GetInstance().GetMainCamera(),
			&cameraPos);
		cameraPos.fX = cameraPos.fX + sign*step;
		orxCamera_SetPosition(OrxCraft::GetInstance().GetMainCamera(),
			&cameraPos);
	    } else if (orxString_Compare (
			orxObject_GetName(pickedObject), "ObjectBarY") == 0 ||
		    orxString_Compare(
			orxObject_GetName(pickedObject), "ObjectBarBgY") == 0 ) {
		orxCamera_GetPosition(OrxCraft::GetInstance().GetMainCamera(),
			&cameraPos);
		cameraPos.fY = cameraPos.fY + sign*step;
		orxCamera_SetPosition(OrxCraft::GetInstance().GetMainCamera(),
			&cameraPos);
	    }
	}
	UpdateScrollBars();
    }
}

void ScrollGUICEGUI::InputMouseUp ()
{
    orxVECTOR mousePos;
    orxMouse_GetPosition (&mousePos);

    CEGUI::System::getSingleton ().injectMousePosition (mousePos.fX, mousePos.fY);
    CEGUI::System::getSingleton ().injectMouseButtonUp (CEGUI::LeftButton);
}

void ScrollGUICEGUI::InputKeyPress (const orxSTRING orxKey)
{
    using CEGUI::Key;
    int key = 0;
    char inputChar = '\0';

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
    if (orxString_Compare (orxKey, "Char0") == 0)
    {
	key = Key::Zero;
	inputChar = '0';
    }
    if (orxString_Compare (orxKey, "Char1") == 0)
    {
	key = Key::One;
	inputChar = '1';
    }
    if (orxString_Compare (orxKey, "Char2") == 0)
    {
	key = Key::Two;
	inputChar = '2';
    }
    if (orxString_Compare (orxKey, "Char3") == 0)
    {
	key = Key::Three;
	inputChar = '3';
    }
    if (orxString_Compare (orxKey, "Char4") == 0)
    {
	key = Key::Four;
	inputChar = '4';
    }
    if (orxString_Compare (orxKey, "Char5") == 0)
    {
	key = Key::Five;
	inputChar = '5';
    }
    if (orxString_Compare (orxKey, "Char6") == 0)
    {
	key = Key::Six;
	inputChar = '6';
    }
    if (orxString_Compare (orxKey, "Char7") == 0)
    {
	key = Key::Seven;
	inputChar = '7';
    }
    if (orxString_Compare (orxKey, "Char8") == 0)
    {
	key = Key::Eight;
	inputChar = '8';
    }
    if (orxString_Compare (orxKey, "Char9") == 0)
    {
	key = Key::Nine;
	inputChar = '9';
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
    if (orxString_Compare (orxKey, "Subtract") == 0)
    {
	key = Key::Subtract;
	inputChar = '-';
    }
    if (orxString_Compare (orxKey, "Period") == 0)
    {
	key = Key::Period;
	inputChar = '.';
    }
    if (orxString_Compare (orxKey, "Dash") == 0)
    {
	key = Key::Minus;
	inputChar = '-';
    }

    CEGUI::System::getSingleton ().injectKeyDown (key);
    if (inputChar != '\0')
    {
	CEGUI::System::getSingleton ().injectChar (inputChar);
    }

    //orxCamera_SetZoom(m_spriteSheetCamera, zoom);
}

orxBOOL ScrollGUICEGUI::OnRender ()
{
    DrawGrid ();
    CEGUI::System::getSingleton().renderGUI();
    return false; 
}

void ScrollGUICEGUI::DrawGrid ()
{
    orxConfig_PushSection ("MainCamera");
    float frustumMainWidth = orxConfig_GetFloat ("FrustumWidth");
    float frustumMainHeight = orxConfig_GetFloat ("FrustumHeight");
    orxConfig_PopSection ();

    orxConfig_PushSection ("SpriteSheetCamera");
    float frustumSheetWidth = orxConfig_GetFloat ("FrustumWidth");
    float frustumSheetHeight = orxConfig_GetFloat ("FrustumHeight");
    orxConfig_PopSection ();

    int gridRes = 100;
    int columnsMain = static_cast<int> (frustumMainWidth / gridRes);
    int rowsMain = static_cast<int> (frustumMainHeight / gridRes);
    int columnsSheet = static_cast<int> (frustumSheetWidth / gridRes);
    int rowsSheet = static_cast<int> (frustumSheetHeight / gridRes);

    orxRGBA gridMainColor = orx2RGBA (200, 200, 200, 255);
    orxRGBA gridSheetColor = orx2RGBA (0, 200, 200, 255);

    for (int i = 1; i <= columnsMain; i++)
    {
	orxVECTOR start = { static_cast<float> (i) * gridRes, 0, 0 };
	orxVECTOR end   = { static_cast<float> (i) * gridRes, frustumMainHeight,
			    0 };
	orxDisplay_DrawLine (&start, &end, gridMainColor);
    }

    for (int i = 1; i <= rowsMain; i++)
    {
	orxVECTOR start = { 0, static_cast<float> (i) * gridRes, 0 };
	orxVECTOR end   = { frustumMainWidth, static_cast<float> (i) * gridRes, 0 };
	orxDisplay_DrawLine (&start, &end, gridMainColor);
    }

    for (int i = 1; i <= columnsSheet; i++)
    {
	orxVECTOR start = { static_cast<float> (i) * gridRes, 0, 10 };
	orxVECTOR end   = { static_cast<float> (i) * gridRes, frustumSheetHeight,
			    10 };
	orxDisplay_DrawLine (&start, &end, gridSheetColor);
    }

    for (int i = 1; i <= rowsSheet; i++)
    {
	orxVECTOR start = { 0, static_cast<float> (i) * gridRes, 10 };
	orxVECTOR end   = { frustumSheetWidth, static_cast<float> (i) * gridRes, 10 };
	orxDisplay_DrawLine (&start, &end, gridSheetColor);
    }
}

void ScrollGUICEGUI::UpdateScrollBars ()
{
    orxFLOAT thickness = 14.0;

    orxVECTOR       vWorldSize;
    orxVECTOR       vViewSize;
    orxVECTOR       vCameraPos;
    orxAABOX        stFrustum;
    orxFLOAT        zoom;
    // Gets camera frustum
    orxCamera_GetFrustum(m_sheetCamera, &stFrustum);
    orxCamera_GetPosition(m_sheetCamera, &vCameraPos);
    zoom = orxCamera_GetZoom(m_sheetCamera);
    // Gets parent size
    orxVector_Sub(&vWorldSize, &(stFrustum.vBR), &(stFrustum.vTL));
    orxVector_Divf(&vViewSize, &vWorldSize, zoom);
    
    orxVECTOR barPosX = {vCameraPos.fX, vWorldSize.fY/2.0-thickness/2.0, 1.0};
    orxVECTOR barSizeX = {vViewSize.fX, thickness, 1};
    orxVECTOR barBgPosX = {0.0, vWorldSize.fY/2.0-thickness/2.0, 1.0};
    orxVECTOR barBgSizeX = {vWorldSize.fX, thickness, 1};
    orxVECTOR barPosY = {-vWorldSize.fX/2.0+thickness/2.0, vCameraPos.fY, 1.0};
    orxVECTOR barSizeY = {thickness, vViewSize.fY, 1};
    orxVECTOR barBgPosY = {-vWorldSize.fX/2.0+thickness/2.0, 0.0, 1.0};
    orxVECTOR barBgSizeY = {thickness, vWorldSize.fY, 1};
    orxObject_SetPosition(m_sheetBarX, &barPosX);
    orxObject_SetScale(m_sheetBarX, &barSizeX);
    orxObject_SetPosition(m_sheetBarBgX, &barBgPosX);
    orxObject_SetScale(m_sheetBarBgX, &barBgSizeX);
    orxObject_SetPosition(m_sheetBarY, &barPosY);
    orxObject_SetScale(m_sheetBarY, &barSizeY);
    orxObject_SetPosition(m_sheetBarBgY, &barBgPosY);
    orxObject_SetScale(m_sheetBarBgY, &barBgSizeY);

    // Gets camera frustum
    orxCamera_GetFrustum(OrxCraft::GetInstance().GetMainCamera(), &stFrustum);
    orxCamera_GetPosition(OrxCraft::GetInstance().GetMainCamera(), &vCameraPos);
    zoom = orxCamera_GetZoom(OrxCraft::GetInstance().GetMainCamera());
    // Gets parent size
    orxVector_Sub(&vWorldSize, &(stFrustum.vBR), &(stFrustum.vTL));
    orxVector_Divf(&vViewSize, &vWorldSize, zoom);
    
    orxVector_Set(&barPosX, vCameraPos.fX, vWorldSize.fY/2.0-thickness/2.0, 1.0);
    orxVector_Set(&barSizeX, vViewSize.fX, thickness, 1);
    orxVector_Set(&barBgPosX, 0.0, vWorldSize.fY/2.0-thickness/2.0, 1.0);
    orxVector_Set(&barBgSizeX, vWorldSize.fX, thickness, 1);
    orxVector_Set(&barPosY, -vWorldSize.fX/2.0+thickness/2.0, vCameraPos.fY, 1.0);
    orxVector_Set(&barSizeY, thickness, vViewSize.fY, 1);
    orxVector_Set(&barBgPosY, -vWorldSize.fX/2.0+thickness/2.0, 0.0, 1.0);
    orxVector_Set(&barBgSizeY, thickness, vWorldSize.fY, 1);
    orxObject_SetPosition(m_objectBarX, &barPosX);
    orxObject_SetScale(m_objectBarX, &barSizeX);
    orxObject_SetPosition(m_objectBarBgX, &barBgPosX);
    orxObject_SetScale(m_objectBarBgX, &barBgSizeX);
    orxObject_SetPosition(m_objectBarY, &barPosY);
    orxObject_SetScale(m_objectBarY, &barSizeY);
    orxObject_SetPosition(m_objectBarBgY, &barBgPosY);
    orxObject_SetScale(m_objectBarBgY, &barBgSizeY);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
