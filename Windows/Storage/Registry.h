//============
// Registry.h
//============

#pragma once


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