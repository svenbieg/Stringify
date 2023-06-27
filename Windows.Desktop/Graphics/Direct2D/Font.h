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
	Font(Handle<String> Family="Segoe UI", UINT Size=14, UINT Weight=400);

	// Common
	IDWriteTextFormat* Get();
	UINT GetSize()const override;

private:
	// Common
	LOGFONT cInfo;
	ComPointer<IDWriteTextFormat> pFormat;
};

}}