//================
// RenderTarget.h
//================

#pragma once


//=======
// Using
//=======

#include "Collections/map.hpp"
#include "Graphics/Direct2D/D2DFactory.h"
#include "Graphics/Direct2D/DWriteFactory.h"
#include "Graphics/Bitmap.h"
#include "Graphics/Brush.h"
#include "Graphics/Font.h"
#include "ComPointer.h"


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
	using D2DFactory=Graphics::Direct2D::D2DFactory;
	using DWriteFactory=Graphics::Direct2D::DWriteFactory;

	// Con-/Destructors
	~RenderTarget();
	static inline Handle<RenderTarget> Create() { return new RenderTarget(); }

	// Common
	VOID BeginDraw(HDC DeviceContext, RECT const& Rect);
	VOID Clip(POINT const& Offset, RECT const& Clip);
	VOID DrawBitmap(RECT const& Destination, Bitmap* Bitmap, RECT const& Source);
	VOID DrawLine(POINT const& From, POINT const& To, Brush* Brush, UINT Width=1);
	VOID DrawPolygon(POINT const* Points, UINT Count, Brush* Brush, UINT Width=1);
	VOID DrawRect(RECT const& Rect, Brush* Brush, UINT Width=1);
	VOID DrawText(RECT const& Rect, FLOAT Scale, Font* Font, Brush* Brush, LPCTSTR Text, UINT Length=0);
	VOID EndDraw();
	VOID FillPolygon(POINT const* Points, UINT Count, Brush* Brush);
	VOID FillRect(RECT const& Rect, Brush* Brush);
	POINT GetOffset() { return m_Offset; }
	SIZE MeasureText(Font* Font, FLOAT Scale, LPCTSTR Text, UINT Length=0);
	VOID Unclip();

private:
	// Con-/Destructors
	RenderTarget();

	// Common
	D2D_POINT_2F D2DPoint(POINT const& Point);
	VOID OnBitmapChanged(Bitmap* Bitmap);
	VOID OnBrushChanged(Brush* Brush);
	ID2D1Bitmap* GetBitmap(Bitmap* Bitmap);
	ID2D1SolidColorBrush* GetBrush(Brush* Brush);
	Collections::map<Bitmap*, ID2D1Bitmap*> m_Bitmaps;
	Collections::map<Brush*, ID2D1SolidColorBrush*> m_Brushes;
	Handle<D2DFactory> m_D2DFactory;
	Handle<DWriteFactory> m_DWriteFactory;
	POINT m_Offset;
	ComPointer<ID2D1DCRenderTarget> m_Target;
};

}