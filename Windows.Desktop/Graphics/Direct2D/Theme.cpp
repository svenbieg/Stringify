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
if(!hCurrent)
	{
	hCurrent=new Theme();
	Default=hCurrent;
	}
return hCurrent;
}


//==========================
// Con-/Destructors Private
//==========================

Theme::Theme()
{
// Brushes
BorderBrush=new D2DBrush(COLOR(0xA0, 0xA0, 0xA0));
ControlBrush=new D2DBrush(SystemColor(COLOR_BTNFACE));
FocusBrush=new D2DBrush(SystemColor(COLOR_3DHIGHLIGHT));
FocusBorderBrush=new D2DBrush(SystemColor(COLOR_3DLIGHT));
GrayTextBrush=new D2DBrush(SystemColor(COLOR_GRAYTEXT));
HighlightBrush=new D2DBrush(Colors::Highlight);
HighlightBorderBrush=new D2DBrush(Colors::HighlightBorder);
InactiveHighlightBrush=new D2DBrush(Colors::Silver);
LightBorderBrush=new D2DBrush(COLOR(0xE0, 0xE0, 0xE0));
LightControlBrush=new D2DBrush(Colors::WhiteGray);
LightTextBrush=new D2DBrush(COLOR(0xA0, 0xA0, 0xA0));
TextBrush=new D2DBrush(SystemColor(COLOR_MENUTEXT));
WindowBrush=new D2DBrush(SystemColor(COLOR_WINDOW));
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

Handle<Theme> Theme::hCurrent;

}}