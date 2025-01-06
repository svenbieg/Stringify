//==========
// Font.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Font.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

IDWriteTextFormat* Font::GetFormat()
{
if(m_Format)
	return m_Format;
if(!m_Factory)
	m_Factory=DWriteFactory::Get();
m_Format=m_Factory->CreateTextFormat(m_Info);
m_Format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
return m_Format;
}

UINT Font::GetSize()const
{
INT height=m_Info.lfHeight;
if(height<0)
	height*=-1;
return height;
}


//==========================
// Con-/Destructors Private
//==========================

Font::Font(LPCSTR family, UINT size, UINT weight):
m_Format(nullptr),
m_Info({ 0 })
{
StringHelper::Copy(m_Info.lfFaceName, 32, family);
m_Info.lfHeight=size;
m_Info.lfWeight=weight;
}

}}