//========
// Font.h
//========

#pragma once


//=======
// Using
//=======

#include <dwrite.h>
#include "Graphics/Font.h"


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
	Font(LPCSTR Family="Segoe UI", UINT Size=14, UINT Weight=400);

	// Common
	IDWriteTextFormat* GetFormat();
	UINT GetSize()const override;

private:
	// Common
	ComPointer<IDWriteTextFormat> m_Format;
	LOGFONT m_Info;
};

}}