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

namespace orx_config_util
{

void BoolToString (orxBOOL inBool, orxSTRING outString)
{
    // Min buffer size: 6
    if (inBool)
    {
	sprintf (outString, "%s", "true");
    }
    else
    {
	sprintf (outString, "%s", "false");
    }
}

void FloatToString (const orxFLOAT inFloat,
					   orxSTRING outString)
{
    // Min buffer size: 48 (FLT_MAX:~3.4e38 - 39 digits + sign + decimal point
    // + 6 digits after decimal point + null char)
    sprintf (outString, "%f", inFloat);
}

string ListToString (const string& prop)
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

void GetListIntoVector (const orxSTRING key, vector<string> &list)
{
    orxASSERT (key != orxNULL);
    
    int counter = orxConfig_GetListCounter (key);
    // Iterate over all list items
    for (int i = 0; i < counter; i++)
    {
	// Get and store next item
	string property = orxConfig_GetListString (key, i);
	list.push_back (property);
    }
}

void VectorToString (const orxSTRING prop,
				 orxU32 elementNum, orxSTRING outString)
{
    // Min buffer size: 48 (FLT_MAX:~3.4e38 - 39 digits + sign + decimal point
    // + 6 digits after decimal point + null char)
    orxASSERT (elementNum < 3);

    orxVECTOR propertyAsVector;
    orxConfig_GetVector (prop, &propertyAsVector);

    switch (elementNum)
    {
    case 0:
	sprintf (outString, "%f", propertyAsVector.fX);
	break;
    case 1:
	sprintf (outString, "%f", propertyAsVector.fY);
	break;
    case 2:
	sprintf (outString, "%f", propertyAsVector.fZ);
    }
}

void SetList (const string& prop, const string& inputString)
{
    const vector<string>& list = orxcraft_util::StringToList(inputString);
    SetList(prop, list);
}

void SetList (const string& prop, const vector<string>& inputList)
{
    unsigned int count = inputList.size();
    const orxSTRING array[count];

    for(unsigned int i=0; i<count; i++)
    {
	array[i] = inputList[i].c_str();
    }

    orxConfig_SetListString(prop.c_str(), array, count);
}

}   // namespace orx_config_util

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
