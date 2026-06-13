//=================
// DWriteFactory.h
//=================

#pragma once


//=======
// Using
//=======

#include <dwrite.h>
#include "ComPointer.h"
#include "Global.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//================
// DWrite-Factory
//================

class DWriteFactory: public Global<DWriteFactory>
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<DWriteFactory> Create() { return Global::Create(); }

	// Common
	ComPointer<IDWriteTextFormat> CreateTextFormat(LOGFONT const& Info);
	ComPointer<IDWriteTextLayout> CreateTextLayout(LPCWSTR Text, UINT Length, IDWriteTextFormat* Format);

private:
	// Con-/Destructors
	DWriteFactory();

	// Common
	ComPointer<IDWriteFactory> m_Factory;
};

}}