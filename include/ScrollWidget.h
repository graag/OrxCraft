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

#ifndef __SCROLL_WIDGET_H__
#define __SCROLL_WIDGET_H__
/**
 * @file ScrollWidget.h
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */

#include <string>

#include "Scroll.h"

class ScrollFrameWindow;

/*
 * For debug build perform a dynamic_cast.
 * Remember to check that result != NULL.
 * For release build static_cast is used.
 */
#ifdef __orxDEBUG__
#define orxCRAFT_CAST dynamic_cast
#else
#define orxCRAFT_CAST static_cast
#endif

typedef enum __orxCRAFT_WIDGET_DATA_TYPES_t
{
    orxCRAFT_WIDGET_DATA_STRING = 0,
    orxCRAFT_WIDGET_DATA_BOOL,
    orxCRAFT_WIDGET_DATA_INT,
    orxCRAFT_WIDGET_DATA_FLOAT,
    orxCRAFT_WIDGET_DATA_VECTOR0,
    orxCRAFT_WIDGET_DATA_VECTOR1,
    orxCRAFT_WIDGET_DATA_VECTOR2,
    orxCRAFT_WIDGET_DATA_LIST
} orxCRAFT_WIDGET_DATA_TYPES;

/**
 *  Base class for any type of user interface widget
 */
class ScrollWidget
{
public:
    /** Widget C-tor */
    explicit ScrollWidget (ScrollFrameWindow *dialog) :
	m_manager    (dialog)
    {
    }

    /** Initialize the widget.
     *
     * @param[in] widgetName - the name will be used to select proper instace
     * of the toolkit widget
     */
    virtual void Init (const std::string& widgetName)
    {
    	m_widgetUniqueName = widgetName;
    	m_widgetName = ExtractCommonName(widgetName);
    }

    /** Get the name of the widget */
    inline const std::string& GetName () { return m_widgetName; }
    /** Get the unique name of the widget: name prefixed by "ID_". */
    inline const std::string& GetUniqueName () { return m_widgetUniqueName; }
    /** Remove unique prefix from widget name
     * @param[in] widgetName - name of the widget.
     * @return widgetName with "ID_" prefix removed.
     */
    const std::string ExtractCommonName(const std::string& widgetName) const
    { return widgetName.substr(widgetName.find("_") + 1); }
    /** Bind widget to orx config attribute.
     * @param[in] configName - name of orx config attribute
     * @param[in] type - type of data used by orx config attribute. Valid
     *                   values defined by orxCRAFT_WIDGET_DATA_TYPES
     */
    inline void ConfigBind(const std::string& configName,
    	    const orxCRAFT_WIDGET_DATA_TYPES type=orxCRAFT_WIDGET_DATA_STRING)
    { m_configName = configName; m_dataType = type; }
    inline const std::string& GetConfigName() { return m_configName; }
    /** Set wiget data from orx config for currently selected section.
     */
    virtual void ConfigRead() = 0;
    /** Set orx config attribute value for currently selected section based on
     * widget data.
     */
    virtual void ConfigUpdate() = 0;

protected:
    std::string        m_widgetUniqueName;
    std::string        m_widgetName;
    std::string        m_configName;
    ScrollFrameWindow *m_manager;
    orxCRAFT_WIDGET_DATA_TYPES m_dataType;
};

#endif  // __SCROLLWIDGET_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
