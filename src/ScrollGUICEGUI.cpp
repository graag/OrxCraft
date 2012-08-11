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

    // Creates object viewport
    // Viewports are positioned in the order they are created. Oldest one on top.
    m_objectViewport = orxViewport_CreateFromConfig("ObjectViewport");
    // Gets object camera
    m_objectCamera = orxViewport_GetCamera(m_objectViewport);
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

    // Make sure currentSheet and currentObject are initialized to NULL
    m_currentSheet = NULL;
    m_currentObject = NULL;
    // Set currentSheet and currentObject to something meaningful
    m_currentSheet = OrxCraft::GetInstance().GetObjectByName("O-Soldier")->GetOrxObject();
    m_currentObject = OrxCraft::GetInstance().GetObjectByName("O-Soldier2")->GetOrxObject();
}

void ScrollGUICEGUI::OnDelete ()
{
    //m_glRenderer->destroySystem ();
    m_glRenderer = orxNULL;
}

void ScrollGUICEGUI::Update (const orxCLOCK_INFO &_rstInfo)
{
    // Update scroll bars. If object changes size scroll bars will adjust
    //TODO I'm not sure if this is the best solution. Maybe scroll bars should
    //not depend that closely on objects size ??
    UpdateScrollBars();
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

    // Inject Mouse position to check if mouse is over one of GUI windows.
    CEGUI::System::getSingleton ().injectMousePosition (mousePos.fX, mousePos.fY);
    CEGUI::Window *window = CEGUI::System::getSingleton ().getWindowContainingMouse ();

    /* Root window spans whole screen so mouse is always over it. Disregard
     * root window and check for other GUI windows. If mouse is over one of
     * them inject the mouse button.
     */
    if(window != NULL && orxString_Compare(window->getName().c_str(), "root") != 0) {
	CEGUI::System::getSingleton ().injectMouseButtonDown (CEGUI::LeftButton);
    } else {
	//TODO Viewport and ScrollBars should be moved to a separate class ...
	orxVECTOR cameraPos;

	//TODO remove this commented out section when enough testing shows it is indeed not needed.
	/* orxRender_GetWorldPosition should return Z value equal to begining
	 * of top viewport below the screen position. However ScrollEd uses
	 * additional code to do it manualy. I encountered at some point a
	 * situation that object was selected from other vieport (viewports
	 * were showing the same part of the world but with different Z
	 * offsets). Cannot reproduce this anymore though.
	 */
	/*
	orxFLOAT x, y, w, h;
	orxAABOX cameraFrustum;
	orxViewport_GetPosition(m_objectViewport, &x, &y);
	orxViewport_GetSize(m_objectViewport, &w, &h);
	if(mousePos.fX > x && mousePos.fX < x+w && mousePos.fY > y && mousePos.fY < y+h) {
	    orxCamera_GetFrustum(m_objectCamera, &cameraFrustum);
	    worldPos.fZ = cameraFrustum.vTL.fZ +
		orxCamera_GetPosition(m_objectCamera, &cameraPos)->fZ + orx2F(0.0002f);
	} else {
	    orxCamera_GetFrustum(m_sheetCamera, &cameraFrustum);
	    worldPos.fZ = cameraFrustum.vTL.fZ +
		orxCamera_GetPosition(m_sheetCamera, &cameraPos)->fZ + orx2F(0.0002f);
	}*/

	// Picks object
	orxOBJECT *pickedObject = orxObject_Pick(&worldPos);
	// Found?
	if(pickedObject) {
	    orxVECTOR cameraPos;
	    orxFLOAT sign;
	    // Check the direction of camera move. LeftClick (left/up),
	    // Right (right/down).
	    if(orxInput_IsActive("LeftClick") ) {
		sign = -1.0;
	    } else if (orxInput_IsActive("RightClick")) {
		sign =  1.0;
	    }
	    // Scrollbars
	    //TODO add checks that stop movement outside of the world size
	    if (orxString_Compare ( // SpriteSheetViewport - X axis
			orxObject_GetName(pickedObject), "SheetBarX") == 0 ||
		    orxString_Compare(
			orxObject_GetName(pickedObject), "SheetBarBgX") == 0 ) {
		orxCamera_GetPosition(m_sheetCamera, &cameraPos);
		cameraPos.fX = cameraPos.fX + sign*step;
		orxCamera_SetPosition(m_sheetCamera, &cameraPos);
	    } else if (orxString_Compare ( // SpriteSheetViewport - Y axis
			orxObject_GetName(pickedObject), "SheetBarY") == 0 ||
		    orxString_Compare(
			orxObject_GetName(pickedObject), "SheetBarBgY") == 0 ) {
		orxCamera_GetPosition(m_sheetCamera, &cameraPos);
		cameraPos.fY = cameraPos.fY + sign*step;
		orxCamera_SetPosition(m_sheetCamera, &cameraPos);
	    } else if (orxString_Compare ( // ObjectViewport - X axis
			orxObject_GetName(pickedObject), "ObjectBarX") == 0 ||
		    orxString_Compare(
			orxObject_GetName(pickedObject), "ObjectBarBgX") == 0 ) {
		orxCamera_GetPosition(m_objectCamera, &cameraPos);
		cameraPos.fX = cameraPos.fX + sign*step;
		orxCamera_SetPosition(m_objectCamera, &cameraPos);
	    } else if (orxString_Compare ( // ObjectViewport - Y axis
			orxObject_GetName(pickedObject), "ObjectBarY") == 0 ||
		    orxString_Compare(
			orxObject_GetName(pickedObject), "ObjectBarBgY") == 0 ) {
		orxCamera_GetPosition(m_objectCamera, &cameraPos);
		cameraPos.fY = cameraPos.fY + sign*step;
		orxCamera_SetPosition(m_objectCamera, &cameraPos);
	    }
	}
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

    orxVECTOR       vCameraSize;
    orxVECTOR       vViewSize;
    orxVECTOR       vWorldSize;
    orxVECTOR       vCameraPos;
    orxVECTOR       vBarSize;
    orxAABOX        stFrustum;
    orxFLOAT        zoom;

    // Gets camera frustum, position and zoom
    orxCamera_GetFrustum(m_sheetCamera, &stFrustum);
    orxCamera_GetPosition(m_sheetCamera, &vCameraPos);
    zoom = orxCamera_GetZoom(m_sheetCamera);
    // Gets camera size
    orxVector_Sub(&vCameraSize, &(stFrustum.vBR), &(stFrustum.vTL));
    /* Calculate world size. World is defined by the active object. It has
     * origin in X=0, Y=0 and spans so that the object is completely contained.
     * For object of size (10,10) positioned at (20,20) the world will span
     * from (-30,30) to (30,30). This should take into account: pivot,
     * rotation, zoom and scale.
     */
    //TODO I'm not sure if it should be so closely related to object size. The
    //(0,0) origin is nice to observe rotations around pivot that is not
    //centered at the object centre (animations, FXs). The size of the world
    //coud be however e.g 2x current size, and it would be adjusted only when
    //object gets outside of the BBox
    if(m_currentSheet != NULL) {
	orxOBOX box;
	orxAABOX bbox;
	// Calculate the bounding box of the object. This takes care of scale
	// and rotations.
	orxObject_GetBoundingBox(m_currentSheet, &box);
	orxOBox_GetExtrema(&bbox, &box);
	// Set world size to 2*maximum distance of objects BBox from world
	// origin in X and Y
	if(orxMath_Abs(bbox.vTL.fX) > orxMath_Abs(bbox.vBR.fX))
	    vWorldSize.fX = orxMath_Abs(bbox.vTL.fX)*2.0;
	else
	    vWorldSize.fX = orxMath_Abs(bbox.vBR.fX)*2.0;
	if(orxMath_Abs(bbox.vTL.fY) > orxMath_Abs(bbox.vBR.fY))
	    vWorldSize.fY = orxMath_Abs(bbox.vTL.fY)*2.0;
	else
	    vWorldSize.fY = orxMath_Abs(bbox.vBR.fY)*2.0;

	// Make sure we can divide by world size. fZ cannot == 0.
	vWorldSize.fZ = 1.0;
	// Adjust for camera zoom
	orxVector_Mulf(&vWorldSize, &vWorldSize, zoom);
	// Calculate scroll bar sizes. It is fraction of camera size equal to
	// ratio of camera to world size
	orxVector_Mul(&vBarSize, &vCameraSize, &vCameraSize);
	orxVector_Div(&vBarSize, &vBarSize, &vWorldSize);
	orxVector_Div(&vWorldSize, &vWorldSize, &vCameraSize);
	// Make sure that scroll bars are not to large
	if(vBarSize.fX > vCameraSize.fX)
	    vBarSize.fX = vCameraSize.fX;
	if(vBarSize.fY > vCameraSize.fY)
	    vBarSize.fY = vCameraSize.fY;
    } else {
	orxVector_Copy(&vBarSize, &vCameraSize);
    }

    /* Calculate scroll bars position and size. Take into account thickness of
     * the other scroll bar (dead area where scroll bars would intersect).
     * Scrollbars have defined cameras as thier parents. Therefore positions
     * are defined in the parent space with origin (0,0) being at camera
     * center. Scroll bar position is equal to ratio of camera position (taking
     * into account zoom) to world size.
     */
    orxVECTOR barPosX = {vCameraPos.fX*zoom/vWorldSize.fX+thickness/2.0, vCameraSize.fY/2.0-thickness/2.0, 0.001};
    orxVECTOR barSizeX = {vBarSize.fX-thickness, thickness, 1};
    orxVECTOR barBgPosX = {thickness/2.0, vCameraSize.fY/2.0-thickness/2.0, 0.002};
    orxVECTOR barBgSizeX = {vCameraSize.fX-thickness, thickness, 1};
    orxVECTOR barPosY = {-vCameraSize.fX/2.0+thickness/2.0, vCameraPos.fY*zoom/vWorldSize.fY-thickness/2.0, 0.001};
    orxVECTOR barSizeY = {thickness, vBarSize.fY-thickness, 1};
    orxVECTOR barBgPosY = {-vCameraSize.fX/2.0+thickness/2.0, -thickness/2.0, 0.002};
    orxVECTOR barBgSizeY = {thickness, vCameraSize.fY-thickness, 1};
    orxObject_SetPosition(m_sheetBarX, &barPosX);
    orxObject_SetScale(m_sheetBarX, &barSizeX);
    orxObject_SetPosition(m_sheetBarBgX, &barBgPosX);
    orxObject_SetScale(m_sheetBarBgX, &barBgSizeX);
    orxObject_SetPosition(m_sheetBarY, &barPosY);
    orxObject_SetScale(m_sheetBarY, &barSizeY);
    orxObject_SetPosition(m_sheetBarBgY, &barBgPosY);
    orxObject_SetScale(m_sheetBarBgY, &barBgSizeY);

    // Gets camera frustum
    orxCamera_GetFrustum(m_objectCamera, &stFrustum);
    orxCamera_GetPosition(m_objectCamera, &vCameraPos);
    zoom = orxCamera_GetZoom(m_objectCamera);
    // Gets parent size
    orxVector_Sub(&vCameraSize, &(stFrustum.vBR), &(stFrustum.vTL));
    // orxVector_Divf(&vViewSize, &vCameraSize, zoom); 
    if(m_currentObject != NULL) {
	orxOBOX box;
	orxAABOX bbox;
	orxObject_GetBoundingBox(m_currentObject, &box);
	orxOBox_GetExtrema(&bbox, &box);
	if(orxMath_Abs(bbox.vTL.fX) > orxMath_Abs(bbox.vBR.fX))
	    vWorldSize.fX = orxMath_Abs(bbox.vTL.fX)*2.0;
	else
	    vWorldSize.fX = orxMath_Abs(bbox.vBR.fX)*2.0;
	if(orxMath_Abs(bbox.vTL.fY) > orxMath_Abs(bbox.vBR.fY))
	    vWorldSize.fY = orxMath_Abs(bbox.vTL.fY)*2.0;
	else
	    vWorldSize.fY = orxMath_Abs(bbox.vBR.fY)*2.0;

	float x = vWorldSize.fX;
	float y = vWorldSize.fY;
	vWorldSize.fZ = 1.0;
	orxVector_Mulf(&vWorldSize, &vWorldSize, zoom);
	x = vWorldSize.fX;
	y = vWorldSize.fY;
	orxVector_Mul(&vBarSize, &vCameraSize, &vCameraSize);
	orxVector_Div(&vBarSize, &vBarSize, &vWorldSize);
	orxVector_Div(&vWorldSize, &vWorldSize, &vCameraSize);
	if(vBarSize.fX > vCameraSize.fX)
	    vBarSize.fX = vCameraSize.fX;
	if(vBarSize.fY > vCameraSize.fY)
	    vBarSize.fY = vCameraSize.fY;
    } else {
	orxVector_Copy(&vBarSize, &vCameraSize);
    }
    
    orxVector_Set(&barPosX, vCameraPos.fX*zoom/vWorldSize.fX+thickness/2.0, vCameraSize.fY/2.0-thickness/2.0, 0.001);
    orxVector_Set(&barSizeX, vBarSize.fX-thickness, thickness, 1);
    orxVector_Set(&barBgPosX, thickness/2.0, vCameraSize.fY/2.0-thickness/2.0, 0.002);
    orxVector_Set(&barBgSizeX, vCameraSize.fX-thickness, thickness, 1);
    orxVector_Set(&barPosY, -vCameraSize.fX/2.0+thickness/2.0, vCameraPos.fY*zoom/vWorldSize.fY-thickness/2.0, 0.001);
    orxVector_Set(&barSizeY, thickness, vBarSize.fY-thickness, 1);
    orxVector_Set(&barBgPosY, -vCameraSize.fX/2.0+thickness/2.0, -thickness/2.0, 0.002);
    orxVector_Set(&barBgSizeY, thickness, vCameraSize.fY-thickness, 1);
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
