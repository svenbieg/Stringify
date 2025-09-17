//==========
// Bitmap.h
//==========

#pragma once


//=======
// Using
//=======

#include "Graphics/Color.h"
#include "Graphics/Rect.h"
#include "Graphics/Size.h"


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
	~Bitmap();
	static inline Handle<Bitmap> Create(UINT Width, UINT Height, WORD BitsPerPixel)
		{
		return new Bitmap(Width, Height, BitsPerPixel);
		}
	static inline Handle<Bitmap> Create(UINT Width, UINT Height, WORD BitsPerPixel, LPCSTR Resource)
		{
		return new Bitmap(Width, Height, BitsPerPixel, Resource);
		}

	// Common
	inline BYTE const* Begin()const { return m_Buffer; }
	Event<Bitmap> Changed;
	VOID Clear(COLOR Color);
	Handle<Bitmap> Copy()const;
	Event<Bitmap> Destroyed;
	VOID FillRect(RECT const& Rect, COLOR Color);
	WORD GetBitsPerPixel()const { return m_BitsPerPixel; }
	inline SIZE GetDimensions()const { return SIZE(m_Width, m_Height); }
	inline UINT GetHeight()const { return m_Height; }
	COLOR GetPixel(UINT Left, UINT Top)const;
	inline UINT GetSize()const { return m_Size; }
	inline UINT GetWidth()const { return m_Width; }
	VOID SetPixel(UINT Left, UINT Top, COLOR Color);

protected:
	// Common
	UINT Release()override;
	WORD m_BitsPerPixel;
	BYTE* m_Buffer;
	UINT m_Height;
	UINT m_Pitch;
	LPCSTR m_Resource;
	UINT m_Size;
	UINT m_Width;

private:
	// Con-/Destructors
	Bitmap(UINT Width, UINT Height, WORD BitsPerPixel);
	Bitmap(UINT Width, UINT Height, WORD BitsPerPixel, LPCSTR Resource);
};

}