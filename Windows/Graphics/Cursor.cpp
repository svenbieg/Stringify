//============
// Cursor.cpp
//============

#include "Cursor.h"


//===========
// Namespace
//===========

namespace Graphics {


//==================
// Con-/Destructors
//==================

Cursor::~Cursor()
{
DeleteObject(m_Cursor);
m_Cursor=NULL;
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
	m_Cursor=NULL;
if(m_Cursor==NULL)
	throw InvalidArgumentException();
}

}