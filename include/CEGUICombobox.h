#ifndef __CEGUICOMBOBOX_H__
#define __CEGUICOMBOBOX_H__
/**
 * @file CEGUICombobox.h
 * @date 2012-05-11
 * @author fritz@fritzmahnke.com
 *
 */

#include <vector>
#include <string>

#include "CEGUI.h"

#include "ScrollCombobox.h"

class ScrollFrameWindow;

/** CEGUI implementation of a Combobox */
class CEGUICombobox : public ScrollCombobox
{
public:
    /** C-tor */
    explicit CEGUICombobox (ScrollFrameWindow *dialog);
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
    /** Select and item.
     *
     * @param[in] itemText - contents of the item to be selected.
     */
    virtual void SelectItem (const std::string& text);
    /** Get the text of the selected item */
    virtual const std::string GetSelectedItem () const;

private:
    /** Handler for CEGUI::Combobox::EventListSelectionAccepted event.
     *
     * @param[in] e - WindowEventArgs event arguments passed from CEGUI.
     */
    bool OnSelectionAccepted (const CEGUI::EventArgs &e);

    //! Pointer to CEGUI widget
    CEGUI::Combobox *m_ceCombobox;
    //! List of combobox items
    std::vector<CEGUI::ListboxTextItem *> m_items;
};
#endif  // __CEGUICOMBOBOX_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
