//===========
// Theme.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Graphics/Theme.h"


//===========
// Namespace
//===========

namespace Graphics {


//========
// Common
//========

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
}


//============================
// Con-/Destructors Protected
//============================

Theme::Theme(ColorScheme scheme):
m_ColorScheme(ColorScheme::Light)
{
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
SetColorScheme(scheme);
}

}