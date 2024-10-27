//=========
// Theme.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/Theme.h"
#include "Brush.h"
#include "Cursor.h"
#include "Font.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//=======
// Theme
//=======

class Theme: public Graphics::Theme
{
public:
	// Using
	using D2DBrush=Graphics::Direct2D::Brush;
	using D2DCursor=Graphics::Direct2D::Cursor;
	using D2DFont=Graphics::Direct2D::Font;

	// Common
	static Handle<Theme> Get();

	// Fonts
	inline Handle<Font> GetDefaultFont()override { return DefaultFont; }
	Handle<D2DFont> DefaultFont;

	// Cursors
	inline Handle<Cursor> GetDefaultCursor()override { return DefaultCursor; }
	inline Handle<Cursor> GetHandPointCursor()override { return HandPointCursor; }
	inline Handle<Cursor> GetSizeHorizontalCursor()override { return SizeHorizontalCursor; }
	inline Handle<Cursor> GetSizeVerticalCursor()override { return SizeVerticalCursor; }
	inline Handle<Cursor> GetTextCursor()override { return TextCursor; }
	Handle<D2DCursor> DefaultCursor;
	Handle<D2DCursor> HandPointCursor;
	Handle<D2DCursor> SizeHorizontalCursor;
	Handle<D2DCursor> SizeVerticalCursor;
	Handle<D2DCursor> TextCursor;

private:
	// Con-/Destructors
	Theme();

	// Common
	inline COLOR SystemColor(INT Index) { return GetSysColor(Index)|0xFF000000; }
	static Handle<Theme> m_Current;
};

}}