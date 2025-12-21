//===========
// Brush.cpp
//===========

#include "Brush.h"


//===========
// Namespace
//===========

namespace Graphics {


//========
// Common
//========

VOID Brush::SetColor(COLOR c)
	{
	if(m_Color==c)
		return;
	m_Color=c;
	Changed(this);
	}


//==================
// Common Protected
//==================

UINT Brush::Release()
	{
	if(m_ReferenceCount==1)
		Destroyed(this);
	return Object::Release();
	}

}