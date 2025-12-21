//===========
// Theme.cpp
//===========

#include "Graphics/Theme.h"


//=======
// Using
//=======

#include "Storage/Registry.h"

using namespace Storage;


//===========
// Namespace
//===========

namespace Graphics {


//==================
// Con-/Destructors
//==================

Theme::~Theme()
{
if(s_Current==this)
	s_Current=nullptr;
}


//========
// Common
//========

Handle<Theme> Theme::Get()
{
if(!s_Current)
	s_Current=new Theme();
return s_Current;
}

VOID Theme::SetColorScheme(ColorScheme scheme)
{
if(m_ColorScheme==scheme)
	return;
switch(scheme)
	{
	case ColorScheme::Dark:
		{
		BackgroundBrush->SetColor(Colors::DarkBackground);
		BorderBrush->SetColor(Colors::DarkBorder);
		BorderInactiveBrush->SetColor(Colors::DarkBorderInactive);
		ControlBrush->SetColor(Colors::DarkControl);
		FocusBrush->SetColor(Colors::DarkFocus);
		FocusBorderBrush->SetColor(Colors::DarkFocusBorder);
		HighlightBrush->SetColor(Colors::DarkHighlight);
		HighlightBorderBrush->SetColor(Colors::DarkHighlightBorder);
		HighlightInactiveBrush->SetColor(Colors::DarkHighlightInactive);
		TextBrush->SetColor(Colors::DarkText);
		TextInactiveBrush->SetColor(Colors::DarkTextInactive);
		WindowBrush->SetColor(Colors::DarkWindow);
		break;
		}
	case ColorScheme::Light:
		{
		BackgroundBrush->SetColor(Colors::LightBackground);
		BorderBrush->SetColor(Colors::LightBorder);
		BorderInactiveBrush->SetColor(Colors::LightBorderInactive);
		ControlBrush->SetColor(Colors::LightControl);
		FocusBrush->SetColor(Colors::LightFocus);
		FocusBorderBrush->SetColor(Colors::LightFocusBorder);
		HighlightBrush->SetColor(Colors::LightHighlight);
		HighlightBorderBrush->SetColor(Colors::LightHighlightBorder);
		HighlightInactiveBrush->SetColor(Colors::LightHighlightInactive);
		TextBrush->SetColor(Colors::LightText);
		TextInactiveBrush->SetColor(Colors::Gray);
		WindowBrush->SetColor(Colors::LightWindow);
		break;
		}
	}
m_ColorScheme=scheme;
Changed(this);
}

VOID Theme::Update()
{
INT light=Registry::GetValue(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", "AppsUseLightTheme", 1);
SetColorScheme(light? ColorScheme::Light: ColorScheme::Dark);
}


//============================
// Con-/Destructors Protected
//============================

Theme::Theme():
m_ColorScheme(ColorScheme::Light)
{
// Colors
BackgroundBrush=Brush::Create(Colors::LightBackground);
BorderBrush=Brush::Create(Colors::LightBorder);
BorderInactiveBrush=Brush::Create(Colors::LightBorderInactive);
ControlBrush=Brush::Create(Colors::LightControl);
FocusBrush=Brush::Create(Colors::LightFocus);
FocusBorderBrush=Brush::Create(Colors::LightFocusBorder);
HighlightBrush=Brush::Create(Colors::LightHighlight);
HighlightBorderBrush=Brush::Create(Colors::LightHighlightBorder);
HighlightInactiveBrush=Brush::Create(Colors::LightHighlightInactive);
TextBrush=Brush::Create(Colors::LightText);
TextInactiveBrush=Brush::Create(Colors::LightTextInactive);
WindowBrush=Brush::Create(Colors::LightWindow);
// Fonts
DefaultFont=Font::Create();
// Cursors
DefaultCursor=Cursor::Create(IDC_ARROW);
HandPointCursor=Cursor::Create(IDC_HAND);
SizeHorizontalCursor=Cursor::Create(IDC_SIZEWE);
SizeVerticalCursor=Cursor::Create(IDC_SIZENS);
TextCursor=Cursor::Create(IDC_IBEAM);
Update();
}


//================
// Common Private
//================

Theme* Theme::s_Current=nullptr;

}