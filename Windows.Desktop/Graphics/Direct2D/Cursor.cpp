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

Cursor::~Cursor()
{
if(m_Cursor)
	{
	DeleteObject(m_Cursor);
	m_Cursor=NULL;
	}
}


//========
// Common
//========

VOID Cursor::Set()
{
SetCursor(m_Cursor);
}


//==========================
// Con-/Destructors Private
//==========================

Cursor::Cursor(LPCTSTR id):
m_Cursor(NULL)
{
m_Cursor=LoadCursor(NULL, id);
if(m_Cursor==INVALID_HANDLE_VALUE)
	throw InvalidArgumentException();
}

}}