//===================
// DWriteFactory.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#pragma comment(lib, "dwrite.lib")

#include "DWriteFactory.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

VOID DWriteFactory::CreateTextFormat(LOGFONT const& info, IDWriteTextFormat** format)
{
if(!pFactory)
	return;
DWRITE_FONT_WEIGHT weight=(DWRITE_FONT_WEIGHT)info.lfWeight;
DWRITE_FONT_STYLE style=DWRITE_FONT_STYLE_NORMAL;
if(info.lfItalic)
	style=DWRITE_FONT_STYLE_ITALIC;
FLOAT height=(FLOAT)info.lfHeight;
if(height<0)
	height*=-1.f;
WCHAR face_name[32];
StringCopy(face_name, 32, info.lfFaceName);
DWRITE_FONT_STRETCH stretch=DWRITE_FONT_STRETCH_NORMAL;
pFactory->CreateTextFormat(face_name, nullptr, weight, style, DWRITE_FONT_STRETCH_NORMAL, height, L"", format);
}

VOID DWriteFactory::CreateTextLayout(LPCWSTR str, UINT len, IDWriteTextFormat* format, IDWriteTextLayout** layout)
{
if(!pFactory)
	return;
pFactory->CreateTextLayout(str, len, format, 0.f, 0.f, layout);
}

DWriteFactory* DWriteFactory::Open()
{
if(!hCurrent)
	hCurrent=new DWriteFactory();
return hCurrent;
}


//==========================
// Con-/Destructors Private
//==========================

DWriteFactory::DWriteFactory()
{
DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), pFactory.GetUnknownPointer());
}


//================
// Common Private
//================

Handle<DWriteFactory> DWriteFactory::hCurrent;

}}