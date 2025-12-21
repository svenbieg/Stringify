//==============
// Registry.cpp
//==============

#include "Registry.h"


//=======
// Using
//=======

#include "PathHelper.h"


//===========
// Namespace
//===========

namespace Storage {


//========
// Common
//========

INT Registry::GetValue(HKEY root, LPCSTR path, LPCSTR name, INT err)
{
DWORD value=0;
DWORD size=sizeof(DWORD);
if(RegGetValueA(root, path, name, RRF_RT_DWORD, nullptr, &value, &size)!=ERROR_SUCCESS)
	return err;
return (INT)value;
}

}