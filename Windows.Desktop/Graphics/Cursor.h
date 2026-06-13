//==========
// Cursor.h
//==========

#pragma once


//=======
// Using
//=======

#include "Handle.h"


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
	// Friends
	friend Object;

	// Con-/Destructors
	~Cursor();
	static inline Handle<Cursor> Create(LPCTSTR Id)
		{
		return Object::Create<Cursor>(Id);
		}

	// Common
	inline HCURSOR GetHandle()const { return m_Cursor; }
	VOID Set();

private:
	// Con-/Destructors
	Cursor(LPCTSTR Id);

	// Common
	HCURSOR m_Cursor;
};

}