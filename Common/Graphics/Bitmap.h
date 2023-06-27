//==========
// Bitmap.h
//==========

#pragma once


//=======
// Using
//=======

#include "Color.h"
#include "Rect.h"
#include "Size.h"


//===========
// Namespace
//===========

namespace Graphics {


//========
// Bitmap
//========

class Bitmap: public Object
{
public:
	// Con-/Destructors
	Bitmap(UINT Width, UINT Height, WORD BitsPerPixel);
	Bitmap(UINT Width, UINT Height, UINT Size, LPCSTR Resource);
	~Bitmap();

	// Common
	BYTE const* Begin()const;
	VOID Clear(COLOR Color);
	Handle<Bitmap> Copy()const;
	Event<Bitmap> Destroyed;
	VOID FillRect(RECT const& Rect, COLOR Color);
	WORD GetBitsPerPixel()const { return uBitsPerPixel; }
	inline SIZE GetDimensions()const { return SIZE(uWidth, uHeight); }
	inline UINT GetHeight()const { return uHeight; }
	COLOR GetPixel(UINT Left, UINT Top)const;
	inline UINT GetSize()const { return uSize; }
	inline UINT GetWidth()const { return uWidth; }
	VOID SetPixel(UINT Left, UINT Top, COLOR Color);

protected:
	// Common
	BYTE* pBuffer;
	LPCSTR pResource;
	WORD uBitsPerPixel;
	UINT uHeight;
	UINT uPitch;
	UINT uSize;
	UINT uWidth;
};

}