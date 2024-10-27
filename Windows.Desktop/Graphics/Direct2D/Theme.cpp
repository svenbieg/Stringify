//===========
// Theme.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Theme.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

Handle<Theme> Theme::Get()
{
if(!m_Current)
	m_Current=new Theme();
return m_Current;
}


//==========================
// Con-/Destructors Private
//==========================

Theme::Theme()
{
// Brushes
BorderBrush=new Brush(COLOR(0xA0, 0xA0, 0xA0));
ControlBrush=new Brush(SystemColor(COLOR_BTNFACE));
FocusBrush=new Brush(SystemColor(COLOR_3DHIGHLIGHT));
FocusBorderBrush=new Brush(SystemColor(COLOR_3DLIGHT));
GrayTextBrush=new Brush(SystemColor(COLOR_GRAYTEXT));
HighlightBrush=new Brush(Colors::Highlight);
HighlightBorderBrush=new Brush(Colors::HighlightBorder);
InactiveHighlightBrush=new Brush(Colors::Silver);
LightBorderBrush=new Brush(COLOR(0xE0, 0xE0, 0xE0));
LightControlBrush=new Brush(Colors::WhiteGray);
LightTextBrush=new Brush(COLOR(0xA0, 0xA0, 0xA0));
TextBrush=new Brush(SystemColor(COLOR_MENUTEXT));
WindowBrush=new Brush(SystemColor(COLOR_WINDOW));
// Fonts
DefaultFont=new D2DFont();
// Cursors
DefaultCursor=new D2DCursor(IDC_ARROW);
HandPointCursor=new D2DCursor(IDC_HAND);
SizeHorizontalCursor=new D2DCursor(IDC_SIZEWE);
SizeVerticalCursor=new D2DCursor(IDC_SIZENS);
TextCursor=new D2DCursor(IDC_IBEAM);
}


//================
// Common Private
//================

Handle<Theme> Theme::m_Current;

}}
