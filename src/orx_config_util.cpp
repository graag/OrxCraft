/**
 * @file orx_config_util.cpp
 * @date 2012-05-10
 * @author fritz@fritzmahnke.com
 *
 */

#include "orx_config_util.h"

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

orxSTRING ListToString (const orxSTRING prop)
{
    // Allocates memory for the output string
    orxASSERT (prop != orxNULL);

    orxU32 block = 256; // Block size
    orxU32 block_counter = 1; // Number of blocks

    // Allocate memory
    orxSTRING buffer = (orxSTRING) orxMemory_Allocate(block * sizeof(orxCHAR), orxMEMORY_TYPE_TEXT);
    buffer[0] = '\0';
    const orxSTRING separator = " # "; // List separator

    // Iterate through list
    orxS32 counter = orxConfig_GetListCounter (prop);
    for (int i = 0; i < counter; i++)
    {
	const orxSTRING propAsString = orxConfig_GetListString (prop, i);

	// Check that resulting string fits the buffer. 4 - size of separator pluss null char.
	orxU32 size = orxString_GetLength(buffer) + orxString_GetLength(propAsString) + 4;
	if(size > block*block_counter) {
	    // Increase the size of buffer to minimum required block multiples
	    block_counter = (size / (block*block_counter)) + 1;
	    orxSTRING tmp_buffer = (orxSTRING) orxMemory_Allocate(block * block_counter * sizeof(orxCHAR), orxMEMORY_TYPE_TEXT);
	    // Copy buffer contents
	    orxString_Copy(tmp_buffer, buffer);
	    orxString_Delete(buffer);
	    buffer = tmp_buffer;
	}
	// Append to buffer list element
	strcat(buffer, propAsString);
	// At least one more property after this one?
	if (i + 1 < counter)
	{
	    strcat (buffer, separator);
	}
    }

    return buffer;
}

void GetListIntoVector (const orxSTRING key, vector<const orxSTRING> &list)
{
    orxASSERT (key != orxNULL);
    
    int counter = orxConfig_GetListCounter (key);
    // Iterate over all list items
    for (int i = 0; i < counter; i++)
    {
	// Get and store next item
	//TODO Maybe one should use orxString_Duplicate? What will happen when orxConfig is reloaded?
	const orxSTRING property = orxConfig_GetListString (key, i);
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

}   // namespace orx_config_util

// vim: tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab
