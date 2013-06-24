/***********************************************************************
    filename:   CEGUIOpenGLViewportTarget.cpp
    created:    Sun Jan 11 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "cegui/renderer/CEGUIOpenGLViewportTarget.h"
#include "CEGUIRenderQueue.h"
#include "CEGUIGeometryBuffer.h"
#include "cegui/renderer/CEGUIOpenGL.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLViewportTarget::OpenGLViewportTarget(OpenGLRenderer& owner) :
    OpenGLRenderTarget(owner)
{
    // viewport area defaults to whatever the current OpenGL viewport is set to
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    Rect init_area(Vector2(static_cast<float>(vp[0]), static_cast<float>(vp[1])),
                   Size(static_cast<float>(vp[2]), static_cast<float>(vp[3])));

    setArea(init_area);
}

//----------------------------------------------------------------------------//
OpenGLViewportTarget::OpenGLViewportTarget(OpenGLRenderer& owner,
    const Rect& area) :
        OpenGLRenderTarget(owner)
{
    setArea(area);
}

//----------------------------------------------------------------------------//
bool OpenGLViewportTarget::isImageryCache() const
{
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
