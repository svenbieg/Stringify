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
	// Con-/Destructors
	~Cursor();
	static inline Handle<Cursor> Create(LPCTSTR Id) { return new Cursor(Id); }

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