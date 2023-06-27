//==========
// Cursor.h
//==========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {


//========
// Cursor
//========

class Cursor: public Object
{
public:
	// Common
	virtual VOID Set()=0;

protected:
	// Con-/Destructors
	Cursor() {}
};

}