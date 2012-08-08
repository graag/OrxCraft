#ifndef SCROLLGUICEGUI_H_
#define SCROLLGUICEGUI_H_
/**
 * @file ScrollGUICEGUI.h
 * @date 2012-06-08
 * @author fritz@fritzmahnke.com
 *
 * The CEGUI implementation of a Scroll GUI
 */
#include "Scroll.h"

#include "CEGUI.h"
#include "RendererModules/OpenGL/CEGUIOpenGLRenderer.h"

/**
 *  Renders GUI items and sends input from Scroll to them.
 */
class ScrollGUICEGUI : public ScrollObject
{
public:
    ScrollGUICEGUI ();

    void InputMouseMove ();
    void InputMouseDown ();
    void InputMouseUp ();
    void InputKeyPress (const orxSTRING orxKey);

private:
    virtual void    OnCreate ();
    virtual void    OnDelete ();
    virtual orxBOOL OnRender ();

    /** Calculate and render the editor grid */
    void DrawGrid ();
    void UpdateScrollBars ();

    CEGUI::OpenGLRenderer *m_glRenderer;
    orxVIEWPORT           *m_sheetViewport;
    orxCAMERA             *m_sheetCamera;
    orxOBJECT             *m_objectBarX;
    orxOBJECT             *m_objectBarBgX;
    orxOBJECT             *m_objectBarY;
    orxOBJECT             *m_objectBarBgY;
    orxOBJECT             *m_sheetBarX;
    orxOBJECT             *m_sheetBarBgX;
    orxOBJECT             *m_sheetBarY;
    orxOBJECT             *m_sheetBarBgY;
};

#endif  // SCROLLGUICEGUI_H_
