#ifndef SCROLLCOMBOBOX_H_
#define SCROLLCOMBOBOX_H_
/**
 * @file ScrollCombobox.h
 * @date 2012-05-11
 * @author fritz@fritzmahnke.com
 *
 */

#include <vector>
#include <string>

#include "ScrollWidget.h"

/**
 *  Base class for a Combobox (drop down list) widget.
 */
class ScrollCombobox : public ScrollWidget
{
public:
    /** C-tor */
    explicit ScrollCombobox (ScrollFrameWindow *dialog) :
	 ScrollWidget (dialog)
    {
    }

    /** Populate the listbox contents.
     *
     * @param[in] listItems - vector of strings to display in the listbox.
     */
    virtual void Fill (const std::vector<std::string> &listItems) = 0;
    /** Select and item.
     *
     * @param[in] itemText - contents of the item to be selected.
     */
    virtual void SelectItem (const std::string& itemText) = 0;
    /** Get the text of the selected item */
    virtual const std::string GetSelectedItem () const = 0;
};

#endif  // SCROLLCOMBOBOX_H_

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
