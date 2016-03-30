// dll_helper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "dll_helper.h"


// This is an example of an exported variable
DLL_HELPER_API int ndll_helper=0;

// This is an example of an exported function.
DLL_HELPER_API int fndll_helper(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see dll_helper.h for the class definition
Cdll_helper::Cdll_helper()
{
	return;
}
