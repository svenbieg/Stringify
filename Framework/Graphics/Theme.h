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

	// Brushes
	Handle<Brush> BorderBrush;
	Handle<Brush> ControlBrush;
	Handle<Brush> FocusBrush;
	Handle<Brush> FocusBorderBrush;
	Handle<Brush> GrayTextBrush;
	Handle<Brush> InactiveHighlightBrush;
	Handle<Brush> HighlightBrush;
	Handle<Brush> HighlightBorderBrush;
	Handle<Brush> LightBorderBrush;
	Handle<Brush> LightControlBrush;
	Handle<Brush> LightTextBrush;
	Handle<Brush> TextBrush;
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
	Theme() {}
};

}