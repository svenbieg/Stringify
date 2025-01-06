//========
// Font.h
//========

#pragma once


//=======
// Using
//=======

#include <dwrite.h>
#include "Graphics/Font.h"
#include "DWriteFactory.h"
#include "ComPointer.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//======
// Font
//======

class Font: public Graphics::Font
{
public:
	// Con-/Destructors
	static inline Handle<Font> Create(LPCSTR Family="Segoe UI", UINT Size=14, UINT Weight=400)
		{
		return new Font(Family, Size, Weight);
		}

	// Common
	IDWriteTextFormat* GetFormat();
	UINT GetSize()const override;

private:
	// Con-/Destructors
	Font(LPCSTR Family, UINT Size, UINT Weight);

	// Common
	Handle<DWriteFactory> m_Factory;
	ComPointer<IDWriteTextFormat> m_Format;
	LOGFONT m_Info;
};

}}