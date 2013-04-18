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

#ifndef __CEGUIEXTENDEDTREEITEM_H__
#define __CEGUIEXTENDEDTREEITEM_H__
/**
 * @file CEGUIExtendedTree.h
 * @date 2013-04-16
 * @author graag@o2.pl
 *
 */

#include "CEGUI.h"

/** Extension of CEGUI Tree widget */
class CEGUIExtendedTreeItem : public CEGUI::TreeItem
{
    public:
	CEGUIExtendedTreeItem(const CEGUI::String& text,
		CEGUI::uint item_id = 0, void* item_data = 0,
		bool disabled = false, bool auto_delete = true);

	void addItem(CEGUIExtendedTreeItem* item);
	void setParent(CEGUIExtendedTreeItem* item) { d_parent = item; }
	CEGUIExtendedTreeItem* getParent(void) { return d_parent; }

	// Unique widget name throughout CEGUI.
	static CEGUI::String WidgetTypeName;

    private:
	CEGUIExtendedTreeItem* d_parent;
};
#endif  // __CEGUIEXTENDEDTREEITEM_H__

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
