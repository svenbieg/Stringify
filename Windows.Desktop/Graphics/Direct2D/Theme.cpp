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

Theme::~Theme()
{
s_Current=nullptr;
}

Handle<Theme> Theme::Get()
{
if(!s_Current)
	s_Current=new Theme();
return s_Current;
}


//==========================
// Con-/Destructors Private
//==========================

Theme::Theme()
{
// Brushes
BorderBrush=Brush::Create(COLOR(0xA0, 0xA0, 0xA0));
ControlBrush=Brush::Create(SystemColor(COLOR_BTNFACE));
FocusBrush=Brush::Create(SystemColor(COLOR_3DHIGHLIGHT));
FocusBorderBrush=Brush::Create(SystemColor(COLOR_3DLIGHT));
GrayTextBrush=Brush::Create(SystemColor(COLOR_GRAYTEXT));
HighlightBrush=Brush::Create(Colors::Highlight);
HighlightBorderBrush=Brush::Create(Colors::HighlightBorder);
InactiveHighlightBrush=Brush::Create(Colors::Silver);
LightBorderBrush=Brush::Create(COLOR(0xE0, 0xE0, 0xE0));
LightControlBrush=Brush::Create(Colors::WhiteGray);
LightTextBrush=Brush::Create(COLOR(0xA0, 0xA0, 0xA0));
TextBrush=Brush::Create(SystemColor(COLOR_MENUTEXT));
WindowBrush=Brush::Create(SystemColor(COLOR_WINDOW));
// Fonts
DefaultFont=D2DFont::Create();
// Cursors
DefaultCursor=D2DCursor::Create(IDC_ARROW);
HandPointCursor=D2DCursor::Create(IDC_HAND);
SizeHorizontalCursor=D2DCursor::Create(IDC_SIZEWE);
SizeVerticalCursor=D2DCursor::Create(IDC_SIZENS);
TextCursor=D2DCursor::Create(IDC_IBEAM);
}


//================
// Common Private
//================

Theme* Theme::s_Current=nullptr;

}}