//=================
// DWriteFactory.h
//=================

#pragma once


//=======
// Using
//=======

#include <dwrite.h>


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//================
// DWrite-Factory
//================

class DWriteFactory: public Object
{
public:
	// Common
	ComPointer<IDWriteTextFormat> CreateTextFormat(LOGFONT const& Info);
	ComPointer<IDWriteTextLayout> CreateTextLayout(LPCWSTR Text, UINT Length, IDWriteTextFormat* Format);
	static Handle<DWriteFactory> Open();

private:
	// Con-/Destructors
	DWriteFactory();

	// Common
	static Handle<DWriteFactory> m_Current;
	ComPointer<IDWriteFactory> m_Factory;
};

}}