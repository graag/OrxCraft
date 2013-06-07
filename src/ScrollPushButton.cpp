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
 * @file ScrollPushButton.cpp
 * @date 2013-03-29
 * @author graag@o2.pl
 *
 */

#include "ScrollPushButton.h"

void ScrollPushButton::ConfigRead()
{
    // Is the widget bound to config key? Return if not.
    if(m_configName.empty())
	return;

    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
            "ScrollPushButton::ConfigRead is not implemented.");
    orxASSERT( false );
}

void ScrollPushButton::ConfigUpdate()
{
    // Is the widget bound to config key? Return if not.
    if(m_configName.empty())
	return;

    orxDEBUG_PRINT(orxDEBUG_LEVEL_USER,
            "ScrollPushButton::ConfigUpdate is not implemented.");
    orxASSERT( false );
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab

