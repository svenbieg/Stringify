//=================
// DWriteFactory.h
//=================

#pragma once


//=======
// Using
//=======

#include <dwrite.h>
#include "ComPointer.h"
#include "Handle.h"


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
	// Con-/Destructors
	~DWriteFactory();
	static Handle<DWriteFactory> Get();

	// Common
	ComPointer<IDWriteTextFormat> CreateTextFormat(LOGFONT const& Info);
	ComPointer<IDWriteTextLayout> CreateTextLayout(LPCWSTR Text, UINT Length, IDWriteTextFormat* Format);

private:
	// Con-/Destructors
	DWriteFactory();

	// Common
	ComPointer<IDWriteFactory> m_Factory;
	static DWriteFactory* s_Current;
};

}}