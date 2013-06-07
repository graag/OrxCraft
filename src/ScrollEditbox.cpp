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
 * @file ScrollEditbox.cpp
 * @date 2013-03-28
 * @author graag@o2.pl
 *
 */

#include "ScrollEditbox.h"

#include <string>

#include "orx_config_util.h"

#include "ScrollFrameWindow.h"

using std::string;

void ScrollEditbox::ConfigRead()
{
    orxASSERT(!m_configName.empty());

    orxU32 id;

    switch(m_dataType) {
	case orxCRAFT_WIDGET_DATA_STRING:
	case orxCRAFT_WIDGET_DATA_STRING_BLOCK:
	    SetText(orxConfig_GetString(m_configName.c_str()));
	    break;
	case orxCRAFT_WIDGET_DATA_FLOAT:
	    SetText(orxConf::GetFloatAsString( m_configName ));
	    break;
	case orxCRAFT_WIDGET_DATA_VECTOR0:
	case orxCRAFT_WIDGET_DATA_VECTOR1:
	case orxCRAFT_WIDGET_DATA_VECTOR2:
	    id = m_dataType - orxCRAFT_WIDGET_DATA_VECTOR0;
	    SetText(orxConf::GetVectorElementAsString(
			m_configName, id
			));
	    break;
	case orxCRAFT_WIDGET_DATA_LIST:
	    SetText(orxConf::GetListAsString( m_configName ));
	    break;
	default:
	    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		    "Unsuported data type: %d.", m_dataType);
	    orxASSERT( false );
    }
}

void ScrollEditbox::ConfigUpdate()
{
    orxASSERT(!m_configName.empty());

    orxU32 id;
    orxSTATUS status = orxSTATUS_FAILURE;
    string value = GetText();

    switch(m_dataType) {
	case orxCRAFT_WIDGET_DATA_STRING:
	    status = orxConfig_SetString(
		    m_configName.c_str(), value.c_str());
	    break;
	case orxCRAFT_WIDGET_DATA_STRING_BLOCK:
	    status = orxConfig_SetStringBlock(
		    m_configName.c_str(), value.c_str());
	    break;
	case orxCRAFT_WIDGET_DATA_FLOAT:
	    status = orxConf::SetFloat(m_configName, value);
	    break;
	case orxCRAFT_WIDGET_DATA_VECTOR0:
	case orxCRAFT_WIDGET_DATA_VECTOR1:
	case orxCRAFT_WIDGET_DATA_VECTOR2:
	    id = m_dataType - orxCRAFT_WIDGET_DATA_VECTOR0;
	    status = orxConf::SetVectorElement(m_configName, id, value);
	    break;
	case orxCRAFT_WIDGET_DATA_LIST:
	    if(value.empty())
		status = orxConfig_ClearValue(m_configName.c_str());
	    else
		status = orxConf::SetList(m_configName, value);
	    break;
	default:
	    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		    "Unsuported data type: %d.", m_dataType);
	    orxASSERT(false);
    }
    if(status != orxSTATUS_SUCCESS)
	orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
		"Failed to update config from widget %s status.",
		m_widgetName.c_str());
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab

