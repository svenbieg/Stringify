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
	// Con-/Destructors
	~Theme();
	static inline Handle<Theme> Create() { return new Theme(); }

	// Common
	Event<Theme> Changed;
	static Handle<Theme> Get();
	inline ColorScheme GetColorScheme()const { return m_ColorScheme; }
	VOID SetColorScheme(ColorScheme Scheme);
	VOID Update();

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
	Theme();

	// Common
	ColorScheme m_ColorScheme;

private:
	// Common
	static Theme* s_Current;
};

}