//========
// Font.h
//========

#pragma once


//=======
// Using
//=======

#include <dwrite.h>
#include "Graphics/Direct2D/DWriteFactory.h"
#include "ComPointer.h"
#include "Handle.h"


//===========
// Namespace
//===========

namespace Graphics {


//======
// Font
//======

class Font: public Object
{
public:
	// Using
	using DWriteFactory=Graphics::Direct2D::DWriteFactory;

	// Con-/Destructors
	static inline Handle<Font> Create(LPCSTR Family="Segoe UI", UINT Size=14, UINT Weight=400)
		{
		return new Font(Family, Size, Weight);
		}

	// Common
	IDWriteTextFormat* GetFormat();
	UINT GetSize()const;

private:
	// Con-/Destructors
	Font(LPCSTR Family, UINT Size, UINT Weight);

	// Common
	Handle<DWriteFactory> m_Factory;
	ComPointer<IDWriteTextFormat> m_Format;
	LOGFONT m_Info;
};

}