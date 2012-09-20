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
#ifndef __orxDEBUG__
#define orxCRAFT_CAST dynamic_cast
#else
#define orxCRAFT_CAST static_cast
#endif

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
    { m_widgetName = widgetName; }

    /** Get the name of the widget */
    inline const std::string& GetName () { return m_widgetName; }

protected:
    std::string        m_widgetName;
    ScrollFrameWindow *m_manager;
};

#endif  // __SCROLLWIDGET_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
