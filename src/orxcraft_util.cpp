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
 * @file orxcraft_util.cpp
 * @date 2012-10-10
 * @author graag@o2.pl
 *
 */

#include "orxcraft_util.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <functional>

#include "ScrollObject.h"

using std::vector;
using std::string;
using std::istringstream;
using std::ostringstream;
using std::getline;
using std::find_if;
using std::not1;
using std::ptr_fun;
using std::isspace;

namespace orxUtil
{

string BoolToString (orxBOOL inBool)
{
    if (inBool)
    {
	return "true";
    }
    else
    {
	return "false";
    }
}

string FloatToString (const orxFLOAT inFloat)
{
    ostringstream buffer;
    buffer<<inFloat;
    return buffer.str();
}

orxBOOL StringToBool( const string& input)
{
    orxBOOL value;
    orxSTATUS result;
    result = orxString_ToBool(input.c_str(), &value, orxNULL);
    orxASSERT(result == orxSTATUS_SUCCESS);
    return value;
}

orxFLOAT StringToFloat( const string& input)
{
    orxFLOAT value;
    orxSTATUS result;
    result = orxString_ToFloat(input.c_str(), &value, orxNULL);
    orxASSERT(result == orxSTATUS_SUCCESS);
    return value;
}

string ListToString(const vector<string>& list,
	const string& separator)
{
    ostringstream buffer;

    int counter = list.size();

    for(int i=0; i<counter; i++) {
	buffer<<list[i];
	if(i + 1 < counter)
	    buffer<<separator;
    }

    return buffer.str();
}

vector<string> StringToList(const string& inputString,
	const char& separator)
{
    vector<string> list;
    string item;
    istringstream ss(inputString);

    while(getline(ss, item, separator))
    {
	list.push_back(Trim(item));
    }

    return list;
}

string LTrim(const string& inputString)
{
    /**
     * Use STL algorithm find_if to locate first non withe space character in
     * the inputString. White space characters are defined by std::isspace()
     * function. Unfortunatelly find_if_not is not yet available. std::isspace
     * is actually overloaded hence we use std::ptr_fun to select the version
     * that does not use locale (hopefully does not affect white spaces). To
     * negate the std::isspace we use std::not1 that negates unary function
     * pointers.
     */
    string::const_iterator start = find_if(
	    inputString.begin(), inputString.end(),
	    not1(ptr_fun<int,int>(isspace)));

    // Return substring based on the iterators.
    return string(start, inputString.end());
}

string RTrim(const string& inputString)
{
    /**
     * Use STL algorithm find_if to locate last non withe space character in
     * the inputString. White space characters are defined by std::isspace()
     * function. Unfortunatelly find_if_not is not yet available. std::isspace
     * is actually overloaded hence we use std::ptr_fun to select the version
     * that does not use locale (hopefully does not affect white spaces). To
     * negate the std::isspace we use std::not1 that negates unary function
     * pointers. We use reverse itertors to iterate from the end of the string.
     * Result of find_if hast to be converted into normal iterator by calling
     * the "base" member method.
     */
    string::const_iterator stop = find_if(
	    inputString.rbegin(), inputString.rend(),
	    not1(ptr_fun<int,int>(isspace))).base();

    // Return substring based on the iterators.
    return string(inputString.begin(), stop);
}

string Trim(const string& inputString)
{
    /**
     * Use STL algorithm find_if to locate first and last non withe space
     * character in the inputString. White space characters are defined by
     * std::isspace() function. Unfortunatelly find_if_not is not yet
     * available. std::isspace is actually overloaded hence we use std::ptr_fun
     * to select the version that does not use locale (hopefully does not
     * affect white spaces). To negate the std::isspace we use std::not1 that
     * negates unary function pointers. For the right hand side we use reverse
     * itertors to iterate from the end of the string.  Result of find_if hast
     * to be converted into normal iterator by calling the "base" member
     * method.
     */
    string::const_iterator start = find_if(
	    inputString.begin(), inputString.end(),
	    not1(ptr_fun<int,int>(isspace)));
    string::const_iterator stop = find_if(
	    inputString.rbegin(), inputString.rend(),
	    not1(ptr_fun<int,int>(isspace))).base();

    // Return substring based on the iterators.
    return string(start, stop);
}

bool IsChild(const ScrollObject* obj, const ScrollObject* parent)
{
    return IsChild(obj->GetOrxObject(), parent->GetOrxObject());
}

bool IsChild(const orxOBJECT* obj, const orxOBJECT* parent)
{
    bool result = false;

    for (orxOBJECT *child = orxObject_GetOwnedChild(parent);
	 child != orxNULL;
	 child = orxObject_GetOwnedSibling(child))
    {
	if(orxString_Compare(
		    orxObject_GetName(child),
		    orxObject_GetName(obj)) == 0
	  )
	{
	    result = true;
	    break;
	}
	if(IsChild(obj, child)) {
	    result = true;
	    break;
	}
    }

    return result;
}

} // namespace orxcraft_util

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
