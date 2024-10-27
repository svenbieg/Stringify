//===========
// Brush.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Brush.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

ComPointer<ID2D1SolidColorBrush> Brush::Get(ID2D1RenderTarget* target, Graphics::Brush* brush)
{
COLOR c=brush->GetColor();
D2D1_COLOR_F color({ (FLOAT)c.GetRed()/255, (FLOAT)c.GetGreen()/255, (FLOAT)c.GetBlue()/255, (FLOAT)c.GetAlpha()/255 });
ID2D1SolidColorBrush* d2d_brush=nullptr;
target->CreateSolidColorBrush(color, &d2d_brush);
return d2d_brush;
}

}}