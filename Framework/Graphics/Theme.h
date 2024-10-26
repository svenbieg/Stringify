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
	virtual Handle<Brush> GetBorderBrush()=0;
	virtual Handle<Brush> GetBrush(COLOR Color)=0;
	virtual Handle<Brush> GetControlBrush()=0;
	virtual Handle<Brush> GetFocusBorderBrush()=0;
	virtual Handle<Brush> GetFocusBrush()=0;
	virtual Handle<Brush> GetGrayTextBrush()=0;
	virtual Handle<Brush> GetHighlightBrush()=0;
	virtual Handle<Brush> GetHighlightBorderBrush()=0;
	virtual Handle<Brush> GetInactiveHighlightBrush()=0;
	virtual Handle<Brush> GetLightBorderBrush()=0;
	virtual Handle<Brush> GetLightControlBrush()=0;
	virtual Handle<Brush> GetLightTextBrush()=0;
	virtual Handle<Brush> GetTextBrush()=0;
	virtual Handle<Brush> GetWindowBrush()=0;

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
