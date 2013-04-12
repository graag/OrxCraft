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
 * @file orx_config_util.cpp
 * @date 2012-05-10
 * @author fritz@fritzmahnke.com
 *
 */

#include "orx_config_util.h"
#include "orxcraft_util.h"

#include <string>
#include <vector>
#include <sstream>

using std::vector;
using std::string;
using std::ostringstream;

namespace orxConf
{

/** Extract config values */

orxBOOL GetBool( const string& prop )
{
    return orxConfig_GetBool(prop.c_str());
}

string GetBoolAsString( const string& prop )
{
    return orxUtil::BoolToString(GetBool(prop));
}

orxFLOAT GetFloat( const string& prop )
{
    return orxConfig_GetFloat(prop.c_str());
}

string GetFloatAsString( const string& prop )
{
    return orxUtil::FloatToString(GetFloat(prop));
}

string GetListAsString( const string& prop )
{
    ostringstream buffer;
    string separator = " # ";

    orxS32 counter = orxConfig_GetListCounter(prop.c_str());

    for(int i=0; i<counter; i++) {
	buffer<<orxConfig_GetListString(prop.c_str(), i);
	if(i + 1 < counter)
	    buffer<<separator;
    }

    return buffer.str();
}

void GetListAsVector( const std::string& prop, vector<string> &list )
{
    int counter = orxConfig_GetListCounter (prop.c_str());
    // Iterate over all list items
    for (int i = 0; i < counter; i++)
    {
	// Get and store next item
	string property = orxConfig_GetListString (prop.c_str(), i);
	list.push_back (property);
    }
}

orxFLOAT GetVectorElement( const string& prop, orxU32 elementNum )
{
    orxASSERT (elementNum < 3);

    orxVECTOR propertyAsVector;
    orxConfig_GetVector (prop.c_str(), &propertyAsVector);

    switch (elementNum)
    {
    case 0:
	return propertyAsVector.fX;
    case 1:
	return propertyAsVector.fY;
    case 2:
	return propertyAsVector.fZ;
    default:
	// We should not end up here
	orxASSERT(false);
	return 0;
    }
}

string GetVectorElementAsString( const string& prop, orxU32 elementNum )
{
    return orxUtil::FloatToString(GetVectorElement(prop, elementNum));
}

std::vector<orxFLOAT> GetVector( const string& prop )
{
    vector<orxFLOAT> vec(3);
    orxVECTOR propertyAsVector;

    orxConfig_GetVector (prop.c_str(), &propertyAsVector);
    vec[0] = propertyAsVector.fX;
    vec[1] = propertyAsVector.fX;
    vec[2] = propertyAsVector.fX;

    return vec;
}

/** Update config values */

orxSTATUS SetBool( const string& prop, orxBOOL value )
{
    return orxConfig_SetBool(prop.c_str(), value);
}

orxSTATUS SetBool( const string& prop, const string& value )
{
    return SetBool(prop, orxUtil::StringToBool(value));
}

orxSTATUS SetFloat( const string& prop, orxFLOAT value )
{
    return orxConfig_SetFloat(prop.c_str(), value);
}

orxSTATUS SetFloat( const string& prop, const string& value )
{
    return SetFloat(prop, orxUtil::StringToFloat(value));
}

orxSTATUS SetVectorElement (const string& prop, orxU32 elementNum, const string& value)
{
    return SetVectorElement(prop, elementNum, orxUtil::StringToFloat(value));
}

orxSTATUS SetVectorElement (const string& prop, orxU32 elementNum, orxFLOAT value)
{
    orxASSERT(elementNum < 3);
    ostringstream buffer;

    orxVECTOR propertyAsVector;
    orxConfig_GetVector(prop.c_str(), &propertyAsVector);

    switch (elementNum)
    {
    case 0:
	propertyAsVector.fX = value;
	break;
    case 1:
	propertyAsVector.fY = value;
	break;
    case 2:
	propertyAsVector.fZ = value;
    }

    return orxConfig_SetVector(prop.c_str(), &propertyAsVector);
}

orxSTATUS SetList (const string& prop, const string& inputString)
{
    const vector<string>& list = orxUtil::StringToList(inputString);
    return SetList(prop, list);
}

orxSTATUS SetList (const string& prop, const vector<string>& inputList)
{
    unsigned int count = inputList.size();
    const orxSTRING array[count];

    for(unsigned int i=0; i<count; i++)
    {
	array[i] = inputList[i].c_str();
    }

    return orxConfig_SetListString(prop.c_str(), array, count);
}

}   // namespace orx_config_util

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
