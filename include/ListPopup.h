/**
 * @file ListPopup.h
 * @date 2012-09-05
 * @author graag@o2.pl
 *
 */
#ifndef __LISTPOPUP_H__
#define __LISTPOPUP_H__

#include <string>

#include "ScrollFrameWindow.h"

class ScrollListbox;

class ListPopup : public ScrollFrameWindow
{
public:
    ListPopup(const std::string& name);

    virtual void Init (const std::string& widgetName);
    virtual void OnMouseClick   (const std::string& widgetName);
    virtual void OnTextAccepted (const std::string& widgetName);
    virtual void OnDestroy ();

private:
    ScrollListbox *m_contentList;
};

#endif	// __LISTPOPUP_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
