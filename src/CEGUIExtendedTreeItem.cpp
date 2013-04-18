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
 * @file CEGUIExtendedTreeItem.cpp
 * @date 2013-04-16
 * @author graag@o2.pl
 *
 */

#include "CEGUIExtendedTreeItem.h"

using CEGUI::Tree;
using CEGUI::TreeItem;
using CEGUI::String;
using CEGUI::uint;

CEGUI::String CEGUIExtendedTreeItem::WidgetTypeName = "CEGUIExtendedTreeItem";

CEGUIExtendedTreeItem::CEGUIExtendedTreeItem(
	const String& text, uint item_id, void* item_data, bool disabled,
	bool auto_delete) :
    TreeItem(text, item_id, item_data, disabled, auto_delete),
    d_parent(NULL)
{
}

void CEGUIExtendedTreeItem::addItem(CEGUIExtendedTreeItem* item)
{
    TreeItem::addItem(item);
    if(item != NULL)
	item->setParent(this);
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
