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
	virtual Handle<Font> GetDefaultFont()=0;

	// Cursors
	virtual Handle<Cursor> GetDefaultCursor()=0;
	virtual Handle<Cursor> GetHandPointCursor()=0;
	virtual Handle<Cursor> GetSizeHorizontalCursor()=0;
	virtual Handle<Cursor> GetSizeVerticalCursor()=0;
	virtual Handle<Cursor> GetTextCursor()=0;

protected:
	// Con-/Destructors
	Theme() {}
};

}
