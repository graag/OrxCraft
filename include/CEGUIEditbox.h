#ifndef __SCROLL_CEGUIEDITBOX_H__
#define __SCROLL_CEGUIEDITBOX_H__
/**
 * @file CEGUIEditbox.h
 * @date 2012-05-08
 * @author fritz@fritzmahnke.com
 *
 */

#include <string>

#include "CEGUI.h"

#include "ScrollEditbox.h"

class ScrollFrameWindow;

/** CEGUI Implementation of an Editbox */
class CEGUIEditbox : public ScrollEditbox
{
public:
    /** C-tor */
    explicit CEGUIEditbox (ScrollFrameWindow *dialog);

    /** Initialize the widget.
     * @param[in] widgetName - the name will be used to select proper instace
     * of the CEGUI widget
     */
    virtual void Init (const std::string& widgetName);

    /** Get the text contained in the Editbox */
    virtual const std::string GetText ();
    /** Set the text contained in the Editbox */
    virtual void SetText (const std::string& text);
private:
    /** Handler for CEGUI::Editbox::EventTextAccepted event
     * (pressed Enter key).
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnTextAccepted (const CEGUI::EventArgs &e);

    //! Pointer to CEGUI widget
    CEGUI::Editbox *m_ceEditbox;
};
#endif  // __SCROLL_CEGUIEDITBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
