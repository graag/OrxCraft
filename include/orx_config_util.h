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

#ifndef ORX_CONFIG_UTIL_H
#define ORX_CONFIG_UTIL_H
/**
 * @file orx_config_util.h
 * @date 2012-05-09
 * @author fritz@fritzmahnke.com
 *
 */

#include "orx/orx.h"

#include <vector>
#include <string>

/**
 *  Facility to help interacting with Orx config files. All the functions
 *  below are designed to work with an Orx config section that has been
 *  previously pushed onto the stack.
 */
namespace orxConf
{
/** Extract config values */

/** Get bool value from orxConfig */
orxBOOL GetBool( const std::string& prop );
/** Get bool value as string from orxConfig */
std::string GetBoolAsString( const std::string& prop );
/** Get float value from orxConfig */
orxFLOAT GetFloat( const std::string& prop );
/** Get bool value as string from orxConfig */
std::string GetFloatAsString( const std::string& prop );
/** Get a list from orxConfig as string */
std::string GetListAsString( const std::string& prop );
/** Get all items in a list from orxConfig into a std::vector */
void GetListAsVector(
	const std::string& prop, std::vector<std::string>& list );
/** Get a vector element from orxConfig */
orxFLOAT GetVectorElement( const std::string& prop, orxU32 elementNum );
/** Get a vector element from orxConfig into a string */
std::string GetVectorElementAsString(
	const std::string& prop, orxU32 elementNum
	);
/** Get a vector from orxConfig as std::vector<float> */
std::vector<orxFLOAT> GetVector( const std::string& prop );

/** Update config values */

/** Set a bool value in orxConfig */
orxSTATUS SetBool( const std::string& prop, orxBOOL value );
/** Set a bool value in orxConfig from string */
orxSTATUS SetBool( const std::string& prop, const std::string& value );
/** Set a float value in orxConfig */
orxSTATUS SetFloat( const std::string& prop, orxFLOAT value );
/** Set a float value in orxConfig from string */
orxSTATUS SetFloat( const std::string& prop, const std::string& value );
/** Set a vector element in orxConfig */
orxSTATUS SetVectorElement(
	const std::string& prop, orxU32 elementNum, orxFLOAT value
	);
/** Set a vector element in orxConfig from string */
orxSTATUS SetVectorElement(
	const std::string& prop, orxU32 elementNum, const std::string& value
	);
/** Set a vector in orxConfig */
orxSTATUS SetVector( const std::string& prop, const std::vector<float>& vec );
/** Set a list in orxConfig */
orxSTATUS SetList(
	const std::string& prop, const std::vector<std::string>& inputList
	);
/** Set a list in orxConfig from string */
orxSTATUS SetList( const std::string& prop, const std::string& inputString );
}   // namespace orx_config_util

#endif // ORX_CONFIG_UTIL_H

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
