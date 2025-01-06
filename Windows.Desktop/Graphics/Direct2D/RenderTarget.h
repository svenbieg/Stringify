//================
// RenderTarget.h
//================

#pragma once


//=======
// Using
//=======

#include "Graphics/RenderTarget.h"
#include "Bitmap.h"
#include "Brush.h"
#include "D2DFactory.h"
#include "Font.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//===============
// Render-Target
//===============

class RenderTarget: public Graphics::RenderTarget
{
public:
	// Con-/Destructors
	static inline Handle<RenderTarget> Create() { return new RenderTarget(); }

	// Common
	VOID BeginDraw(HDC DeviceContext, RECT const& Rect);
	VOID Clip(POINT const& Offset, RECT const& Clip)override;
	VOID DrawBitmap(RECT const& Destination, Handle<Graphics::Bitmap> Bitmap, RECT const& Source)override;
	VOID DrawLine(POINT const& From, POINT const& To, Handle<Graphics::Brush> Brush, UINT Width=1)override;
	VOID DrawPolygon(POINT const* Points, UINT Count, Handle<Graphics::Brush> Brush, UINT Width=1)override;
	VOID DrawRect(RECT const& Rect, Handle<Graphics::Brush> Brush, UINT Width=1)override;
	VOID DrawText(RECT const& Rect, FLOAT Scale, LPCTSTR Text, UINT Length=0)override;
	VOID EndDraw();
	VOID FillPolygon(POINT const* Points, UINT Count, Handle<Graphics::Brush> Brush)override;
	VOID FillRect(RECT const& Rect, Handle<Graphics::Brush> Brush)override;
	POINT GetOffset()override { return m_Offset; }
	SIZE MeasureText(Handle<Graphics::Font> Font, FLOAT Scale, LPCTSTR Text, UINT Length=0)override;
	VOID Unclip()override;

private:
	// Con-/Destructors
	RenderTarget();

	// Common
	D2D_POINT_2F D2DPoint(POINT const& Point);
	Handle<D2DFactory> m_D2DFactory;
	Handle<DWriteFactory> m_DWriteFactory;
	POINT m_Offset;
	ComPointer<ID2D1DCRenderTarget> m_Target;
};

}}