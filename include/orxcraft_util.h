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

#ifndef ORXCRAFT_UTIL_H
#define ORXCRAFT_UTIL_H
/**
 * @file orxcraft_util.h
 * @date 2012-10-10
 * @author graag@o2.pl
 *
 */

#include "orx/orx.h"

#include <vector>
#include <string>

class ScrollObject;

/**
 */
namespace orxUtil
{
/** Convert an Orx bool to an string */
std::string BoolToString( orxBOOL inBool );
/** Convert an Orx float to an string */
std::string FloatToString( orxFLOAT inFloat );
std::string ListToString(const std::vector<std::string>& list,
	const std::string& separator = " # ");
orxBOOL StringToBool( const std::string& input);
orxFLOAT StringToFloat( const std::string& input);
std::vector<std::string> StringToList(const std::string& inputString,
	const char& separator = '#');
std::string LTrim(const std::string& inputString);
std::string RTrim(const std::string& inputString);
std::string Trim(const std::string& inputString);
/**
 * Check if an object is descendant of other object.
 *
 * The comparison DOES NOT check if object instance obj is a descendant of
 * object instance parent. It rather checks that parent has a descendant with
 * the same config name as object. This assumes that child instances are
 * handled automatically by orx while objects them selves are handled by
 * orxCraft.
 *
 * @param[in] obj - Object in question
 * @param[in] parent - Potential parent
 * @return True if parent has a obj as child (recursively), False otherwise
 */
bool IsChild(const ScrollObject* obj, const ScrollObject* parent);
/**
 * Check if an object is descendant of other object.
 *
 * The comparison DOES NOT check if object instance obj is a descendant of
 * object instance parent. It rather checks that parent has a descendant with
 * the same config name as object. This assumes that child instances are
 * handled automatically by orx while objects them selves are handled by
 * orxCraft.
 *
 * @param[in] obj - Object in question
 * @param[in] parent - Potential parent
 * @return True if parent has a obj as child (recursively), False otherwise
 */
bool IsChild(const orxOBJECT* obj, const orxOBJECT* parent);
}   // namespace orxcraft_util

#endif // ORXCRAFT_UTIL_H

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
