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
	Cursor(LPCTSTR Id);
	~Cursor();

	// Common
	HCURSOR GetHandle()const { return hCursor; }
	VOID Set()override;

private:
	// Common
	HCURSOR hCursor;
};

}}