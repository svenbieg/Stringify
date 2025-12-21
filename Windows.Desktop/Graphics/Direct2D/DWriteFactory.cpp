//===================
// DWriteFactory.cpp
//===================

#include "DWriteFactory.h"


//=======
// Using
//=======

#pragma comment(lib, "dwrite.lib")

#include "StringHelper.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//==================
// Con-/Destructors
//==================

DWriteFactory::~DWriteFactory()
{
s_Current=nullptr;
}

Handle<DWriteFactory> DWriteFactory::Get()
{
if(!s_Current)
	s_Current=new DWriteFactory();
return s_Current;
}


//========
// Common
//========

ComPointer<IDWriteTextFormat> DWriteFactory::CreateTextFormat(LOGFONT const& info)
{
DWRITE_FONT_WEIGHT weight=(DWRITE_FONT_WEIGHT)info.lfWeight;
DWRITE_FONT_STYLE style=DWRITE_FONT_STYLE_NORMAL;
if(info.lfItalic)
	style=DWRITE_FONT_STYLE_ITALIC;
FLOAT height=(FLOAT)info.lfHeight;
if(height<0)
	height*=-1.f;
WCHAR face_name[32];
StringHelper::Copy(face_name, 32, info.lfFaceName);
DWRITE_FONT_STRETCH stretch=DWRITE_FONT_STRETCH_NORMAL;
IDWriteTextFormat* format=nullptr;
m_Factory->CreateTextFormat(face_name, nullptr, weight, style, DWRITE_FONT_STRETCH_NORMAL, height, L"", &format);
return format;
}

ComPointer<IDWriteTextLayout> DWriteFactory::CreateTextLayout(LPCWSTR str, UINT len, IDWriteTextFormat* format)
{
IDWriteTextLayout* layout=nullptr;
m_Factory->CreateTextLayout(str, len, format, 0.f, 0.f, &layout);
return layout;
}


//==========================
// Con-/Destructors Private
//==========================

DWriteFactory::DWriteFactory()
{
IDWriteFactory* factory=nullptr;
DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&factory);
m_Factory.Initialize(factory);
}


//================
// Common Private
//================

DWriteFactory* DWriteFactory::s_Current=nullptr;

}}