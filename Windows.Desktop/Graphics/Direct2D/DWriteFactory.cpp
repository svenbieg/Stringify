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

Handle<DWriteFactory> DWriteFactory::Open()
{
if(!m_Current)
	m_Current=new DWriteFactory();
return m_Current;
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

Handle<DWriteFactory> DWriteFactory::m_Current;

}}