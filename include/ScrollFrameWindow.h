#ifndef __SCROLLFRAMEWINDOW_H__
#define __SCROLLFRAMEWINDOW_H__
/**
 * @file ScrollFrameWindow.h
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */
#include <vector>
#include <string>

#include "Scroll.h"
#include "ScrollWidget.h"

class ScrollEditbox;
class ScrollCombobox;
class ScrollListbox;

/**
 *  Interface for a Window.
 */
class ScrollFrameWindow
{
public:
    //! C-tor
    ScrollFrameWindow (const std::string& name);
    
    /** Find a widget by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollWidget pointer if found, NULL otherwise.
     */
    ScrollWidget *FindWidget (const std::string& widgetName) const;
    /** Find a editbox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollEditbox pointer if found, NULL otherwise.
     */
    ScrollEditbox *FindEditbox (const std::string& widgetName) const;
    /** Find a combobox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollCombobox pointer if found, NULL otherwise.
     */
    ScrollCombobox *FindCombobox (const std::string& widgetName) const;
    /** Find a listbox by its name.
     * @param[in] widgetName - name of the widget.
     * @return ScrollListbox pointer if found, NULL otherwise.
     */
    ScrollListbox *FindListbox (const std::string& widgetName) const;

    /** Initialize the window.
     * @param[in] windowName - name of the underlying toolkit window.
     */
    virtual void Init (const std::string& windowName)
    { m_windowName = windowName; }
    /** Get dialog name */
    virtual const std::string& GetName() { return m_name; }
    /** Get the name of underlying toolkit window. */
    virtual const std::string& GetWindowName () { return m_windowName; }
    /** Left mouse click event handler.
     * @param widgetName - name of the widget that originated the event.
     */
    virtual void OnMouseClick   (const std::string& widgetName) = 0;
    /** Text accepted (pressed Enter key) event handler.
     * @param widgetName - name of the widget that originated the event.
     */
    virtual void OnTextAccepted (const std::string& widgetName) = 0;

    /** Add new widget to the window.
     * @param widget - a pointer to ScrollWidget
     */
    virtual void AddWidget (ScrollWidget *widget);

protected:
    //! D-tor
    virtual ~ScrollFrameWindow ()
    {
	std::vector<ScrollWidget *>::iterator iter;
	for (iter = m_widgetList.begin ();
	    iter != m_widgetList.end ();
	    ++iter)
	{
	    delete *iter;
	}
    };

    // Widgets contained in the window
    std::vector<ScrollWidget *> m_widgetList;
    // Name of the underlying toolkit window
    std::string                 m_windowName;
    // Name of the dialog
    std::string                 m_name;
};

#endif  // __SCROLLFRAMEWINDOW_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
