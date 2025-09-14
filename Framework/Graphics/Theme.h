//=========
// Theme.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/Brush.h"
#include "Graphics/Cursor.h"
#include "Graphics/Font.h"


//===========
// Namespace
//===========

namespace Graphics {


//==============
// Color-Scheme
//==============

enum class ColorScheme
{
Dark,
Light
};


//=======
// Theme
//=======

class Theme: public Object
{
public:
	// Using
	using Brush=Graphics::Brush;
	using Font=Graphics::Font;
	using Cursor=Graphics::Cursor;

	// Common
	inline ColorScheme GetColorScheme()const { return m_ColorScheme; }
	VOID SetColorScheme(ColorScheme Scheme);

	// Brushes
	Handle<Brush> BackgroundBrush;
	Handle<Brush> BorderBrush;
	Handle<Brush> BorderInactiveBrush;
	Handle<Brush> ControlBrush;
	Handle<Brush> FocusBrush;
	Handle<Brush> FocusBorderBrush;
	Handle<Brush> HighlightBrush;
	Handle<Brush> HighlightBorderBrush;
	Handle<Brush> HighlightInactiveBrush;
	Handle<Brush> TextBrush;
	Handle<Brush> TextInactiveBrush;
	Handle<Brush> WindowBrush;

	// Fonts
	Handle<Font> DefaultFont;

	// Cursors
	Handle<Cursor> DefaultCursor;
	Handle<Cursor> HandPointCursor;
	Handle<Cursor> SizeHorizontalCursor;
	Handle<Cursor> SizeVerticalCursor;
	Handle<Cursor> TextCursor;

protected:
	// Con-/Destructors
	Theme(ColorScheme Scheme=ColorScheme::Light);

	// Common
	ColorScheme m_ColorScheme;
};

}