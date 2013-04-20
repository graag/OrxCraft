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
 * @file TreePopup.cpp
 * @date 2013-04-15
 * @author graag@o2.pl
 *
 */
#include "TreePopup.h"

#include <string>

#include "orxCraft.h"
#include "orx_config_util.h"
#include "DialogManager.h"
#include "ScrollTreebox.h"

using std::string;

TreePopup::TreePopup(const string& name, const string& title):
    ScrollFrameWindow(name, title),
    m_contentTree(NULL),
    m_parent(NULL),
    m_userData(NULL)
{
}

void TreePopup::Fill (const vector<ScrollTreePair>& dataList)
{
    orxASSERT(m_contentTree != NULL);
    m_contentTree->Fill(dataList);
}

void TreePopup::SetSelection (const vector<string>& selectionList)
{
    orxASSERT(m_contentTree != NULL);
    m_contentTree->SetSelection(selectionList);
}

vector<string> TreePopup::GetSelection()
{
    orxASSERT(m_contentTree != NULL);
    return m_contentTree->GetSelection();
}

void TreePopup::OnMouseClick (const string& widgetName)
{
    orxASSERT (m_contentTree != orxNULL);
    orxASSERT (m_parent != orxNULL);

    if (widgetName == "TreePopup/DoneButton")
    {
	m_parent->OnPopupFinish(m_name, m_title);
	OnDestroy();
    }
}

void TreePopup::OnTextAccepted (const string& widgetName)
{
    orxASSERT (false);
}

void TreePopup::OnPopupFinish (const string& popupName,
	const string& popupTitle)
{
    orxASSERT (false);
}

void TreePopup::OnDestroy ()
{
    OrxCraft::GetInstance().GetDialogManager()->DestroyDialog(m_id);
    /*
     * Beyond this point the dialog was destroyed (delete was issued).
     * Make sure in is not accessed anymore.
     */
}

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
