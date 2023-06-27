//================
// RenderTarget.h
//================

#pragma once


//=======
// Using
//=======

#include "Bitmap.h"
#include "Brush.h"
#include "Color.h"
#include "Font.h"
#include "Point.h"
#include "Rect.h"
#include "Size.h"


//===========
// Namespace
//===========

namespace Graphics {


//===============
// Render-Target
//===============

class RenderTarget: public Object
{
public:
	// Using
	using POINT=Graphics::POINT;
	using RECT=Graphics::RECT;
	using SIZE=Graphics::SIZE;

	// Common
	virtual VOID Clip(POINT const& Offset, RECT const& Clip)=0;
	virtual VOID DrawBitmap(RECT const& Destination, Handle<Bitmap> Bitmap, RECT const& Source)=0;
	virtual VOID DrawLine(POINT const& From, POINT const& To, Handle<Brush> Brush, UINT Width=1)=0;
	virtual VOID DrawPolygon(POINT const* Points, UINT Count, Handle<Graphics::Brush> Brush, UINT Width=1)=0;
	virtual VOID DrawRect(RECT const& Rect, Handle<Brush> Brush, UINT Width=1)=0;
	virtual VOID DrawText(RECT const& Rect, FLOAT Scale, LPCTSTR Text, UINT Length=0)=0;
	virtual VOID FillPolygon(POINT const* Points, UINT Count, Handle<Graphics::Brush> Brush)=0;
	virtual VOID FillRect(RECT const& Rect, Handle<Brush> Brush)=0;
	Handle<Graphics::Font> Font;
	virtual POINT GetOffset()=0;
	virtual SIZE MeasureText(Handle<Graphics::Font> Font, FLOAT Scale, LPCTSTR Text, UINT Length=0)=0;
	Handle<Graphics::Brush> TextColor;
	virtual VOID Unclip() {}

protected:
	// Con-/Destructors
	RenderTarget() {}
};

}