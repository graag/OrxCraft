/**
 * @file InfoWindow.h
 * @date 2012-05-04
 * @author fritz@fritzmahnke.com
 *
 */
#ifndef __INFOWINDOW_H__
#define __INFOWINDOW_H__

#include "ScrollFrameWindow.h"

class InfoWindow : public ScrollFrameWindow
{
public:
    virtual void Init (const std::string& widgetName);
    virtual void OnMouseClick   (const std::string& widgetName);
    virtual void OnTextAccepted (const std::string& widgetName);
};

#endif	// __INFOWINDOW_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
