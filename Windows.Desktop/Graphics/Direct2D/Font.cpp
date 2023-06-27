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

Font::Font(Handle<String> family, UINT size, UINT weight)
{
ZeroMemory(&cInfo, sizeof(LOGFONT));
StringCopy(cInfo.lfFaceName, 32, family->Begin());
cInfo.lfHeight=size;
cInfo.lfWeight=weight;
}


//========
// Common
//========

IDWriteTextFormat* Font::Get()
{
if(pFormat)
	return pFormat.Get();
auto factory=DWriteFactory::Open();
factory->CreateTextFormat(cInfo, pFormat.GetObjectPointer());
pFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
return pFormat.Get();
}

UINT Font::GetSize()const
{
INT height=cInfo.lfHeight;
if(height<0)
	height*=-1;
return height;
}

}}