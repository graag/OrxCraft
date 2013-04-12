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
 * @file ScrollListbox.cpp
 * @date 2013-03-28
 * @author graag@o2.pl
 *
 */

#include <string>
#include <vector>

#include "ScrollListbox.h"

#include "orx_config_util.h"

using std::string;
using std::vector;

void ScrollListbox::ConfigRead()
{
    orxASSERT(!m_configName.empty());
    orxASSERT(m_dataType == orxCRAFT_WIDGET_DATA_LIST);

    vector<string> propList;
    orxConf::GetListAsVector( m_configName, propList );
    SetSelection(propList);
}

void ScrollListbox::ConfigUpdate()
{
    orxASSERT(!m_configName.empty());
    orxASSERT(m_dataType == orxCRAFT_WIDGET_DATA_LIST)

    vector<string> propList = GetSelection();
    orxSTATUS status;

    if(propList.empty())
        orxConfig_ClearValue(m_configName.c_str());
    else
        status = orxConf::SetList(m_configName, propList);

    if(status != orxSTATUS_SUCCESS)
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		"Failed to update config from widget %s status.",
		m_widgetName.c_str());
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab

