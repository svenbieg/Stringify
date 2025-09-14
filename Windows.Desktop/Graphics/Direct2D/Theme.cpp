//===========
// Theme.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "UI/AppWindow.h"
#include "Theme.h"

using namespace Graphics;
using namespace UI;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//==========================
// Con-/Destructors Private
//==========================

Theme::Theme(ColorScheme scheme):
Graphics::Theme(scheme)
{
// Fonts
DefaultFont=D2DFont::Create();
// Cursors
DefaultCursor=D2DCursor::Create(IDC_ARROW);
HandPointCursor=D2DCursor::Create(IDC_HAND);
SizeHorizontalCursor=D2DCursor::Create(IDC_SIZEWE);
SizeVerticalCursor=D2DCursor::Create(IDC_SIZENS);
TextCursor=D2DCursor::Create(IDC_IBEAM);
}

}}