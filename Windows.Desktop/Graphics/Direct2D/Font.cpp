//==========
// Font.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "DWriteFactory.h"
#include "Font.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//==================
// Con-/Destructors
//==================

Font::Font(LPCSTR family, UINT size, UINT weight):
m_Format(nullptr),
m_Info({ 0 })
{
StringCopy(m_Info.lfFaceName, 32, family);
m_Info.lfHeight=size;
m_Info.lfWeight=weight;
}


//========
// Common
//========

IDWriteTextFormat* Font::GetFormat()
{
if(m_Format)
	return m_Format;
auto factory=DWriteFactory::Open();
m_Format=factory->CreateTextFormat(m_Info);
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

}}