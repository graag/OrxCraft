#ifndef __CEGUILISTBOX_H__
#define __CEGUILISTBOX_H__
/**
 * @file CEGUIListbox.h
 * @date 2012-05-10
 * @author fritz@fritzmahnke.com
 *
 */

#include <string>
#include <vector>

#include "CEGUI.h"

#include "ScrollListbox.h"

class ScrollFrameWindow;

/** CEGUI Implementation of a Listbox */
class CEGUIListbox : public ScrollListbox
{
public:
    /** C-tor */
    explicit CEGUIListbox (ScrollFrameWindow *dialog);
    /** Initialize the widget.
     *
     * @param[in] widgetName - the name will be used to select proper instace
     * of the CEGUI widget
     */
    virtual void Init (const std::string& widgetName);
    /** Populate the listbox contents.
     *
     * @param[in] listItems - vector of strings to display in the listbox.
     */
    virtual void Fill (const std::vector<std::string> &listItems);

     //bool OnSelectionChanged (const CEGUI::EventArgs &e);
private:
    /** Handler for CEGUI::Window::EventMouseClick event.
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnMouseClick (const CEGUI::EventArgs &e);

    //! Pointer to CEGUI widget
    CEGUI::Listbox *m_ceListbox;
    //! List of listbox items
    std::vector<CEGUI::ListboxTextItem *> m_items;
};
#endif  // __CEGUILISTBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
