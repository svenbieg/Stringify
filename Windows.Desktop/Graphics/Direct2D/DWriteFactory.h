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
	VOID CreateTextFormat(LOGFONT const& Info, IDWriteTextFormat** Format);
	VOID CreateTextLayout(LPCWSTR Text, UINT Length, IDWriteTextFormat* Format, IDWriteTextLayout** Layout);
	static DWriteFactory* Open();

private:
	// Con-/Destructors
	DWriteFactory();

	// Common
	static Handle<DWriteFactory> hCurrent;
	ComPointer<IDWriteFactory> pFactory;
};

}}