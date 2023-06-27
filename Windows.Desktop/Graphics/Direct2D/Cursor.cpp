//============
// Cursor.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Cursor.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//==================
// Con-/Destructors
//==================

Cursor::Cursor(LPCTSTR id):
hCursor(NULL)
{
hCursor=LoadCursor(NULL, id);
if(hCursor==INVALID_HANDLE_VALUE)
	hCursor=NULL;
}

Cursor::~Cursor()
{
if(hCursor)
	DeleteObject(hCursor);
}


//========
// Common
//========

VOID Cursor::Set()
{
SetCursor(hCursor);
}

}}