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

	// Brushes
	inline Handle<Brush> GetBorderBrush()override { return BorderBrush; }
	inline Handle<Brush> GetBrush(COLOR Color)override { return new D2DBrush(Color); }
	inline Handle<Brush> GetControlBrush()override { return ControlBrush; }
	inline Handle<Brush> GetFocusBrush()override { return FocusBrush; }
	inline Handle<Brush> GetFocusBorderBrush()override { return FocusBorderBrush; }
	inline Handle<Brush> GetGrayTextBrush()override { return GrayTextBrush; }
	inline Handle<Brush> GetHighlightBrush()override { return HighlightBrush; }
	inline Handle<Brush> GetHighlightBorderBrush()override { return HighlightBorderBrush; }
	inline Handle<Brush> GetInactiveHighlightBrush()override { return InactiveHighlightBrush; }
	inline Handle<Brush> GetLightBorderBrush()override { return LightBorderBrush; }
	inline Handle<Brush> GetLightControlBrush()override { return LightControlBrush; }
	inline Handle<Brush> GetLightTextBrush()override { return LightTextBrush; }
	inline Handle<Brush> GetTextBrush()override { return TextBrush; }
	inline Handle<Brush> GetWindowBrush()override { return WindowBrush; }
	Handle<D2DBrush> BorderBrush;
	Handle<D2DBrush> ControlBrush;
	Handle<D2DBrush> FocusBrush;
	Handle<D2DBrush> FocusBorderBrush;
	Handle<D2DBrush> GrayTextBrush;
	Handle<D2DBrush> InactiveHighlightBrush;
	Handle<D2DBrush> HighlightBrush;
	Handle<D2DBrush> HighlightBorderBrush;
	Handle<D2DBrush> LightBorderBrush;
	Handle<D2DBrush> LightControlBrush;
	Handle<D2DBrush> LightTextBrush;
	Handle<D2DBrush> TextBrush;
	Handle<D2DBrush> WindowBrush;

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
	static Handle<Theme> hCurrent;
};

}}