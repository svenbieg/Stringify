//==========
// Cursor.h
//==========

#pragma once


//=======
// Using
//=======

#include "Graphics/Cursor.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Cursor
//========

class Cursor: public Graphics::Cursor
{
public:
	// Con-/Destructors
	~Cursor();
	static inline Handle<Cursor> Create(LPCTSTR Id) { return new Cursor(Id); }

	// Common
	inline HCURSOR GetHandle()const { return m_Cursor; }
	VOID Set()override;

private:
	// Con-/Destructors
	Cursor(LPCTSTR Id);

	// Common
	HCURSOR m_Cursor;
};

}}