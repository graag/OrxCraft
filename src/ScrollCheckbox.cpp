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

/**
 * @file ScrollCheckbox.cpp
 * @date 2013-03-28
 * @author graag@o2.pl
 *
 */

#include "ScrollCheckbox.h"

#include <string>

#include "ScrollFrameWindow.h"

using std::string;

void ScrollCheckbox::ConfigRead()
{
    // Is the widget bound to config key? Return if not.
    if(m_configName.empty())
	return;

    orxASSERT(m_dataType == orxCRAFT_WIDGET_DATA_BOOL)
    SetSelection(orxConfig_GetBool (m_configName.c_str()));
}

void ScrollCheckbox::ConfigUpdate()
{
    // Is the widget bound to config key? Return if not.
    if(m_configName.empty())
	return;

    orxASSERT(m_dataType == orxCRAFT_WIDGET_DATA_BOOL)

    orxSTATUS status;
    status = orxConfig_SetBool (m_configName.c_str(), GetSelection());
    if(status != orxSTATUS_SUCCESS)
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		"Failed to update config from widget %s status.",
		m_widgetName.c_str());
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab

