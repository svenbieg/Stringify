//============
// Registry.h
//============

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"


//===========
// Namespace
//===========

namespace Storage {


//==========
// Registry
//==========

class Registry
{
public:
	// Common
	static INT GetValue(HKEY Root, LPCSTR Path, LPCSTR Name, INT Error=-1);
};

}