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


//==================
// Con-/Destructors
//==================

Brush::Brush(COLOR c):
cColor({ (FLOAT)c.GetRed()/255, (FLOAT)c.GetGreen()/255, (FLOAT)c.GetBlue()/255, (FLOAT)c.GetAlpha()/255 }),
uColor(c)
{}


//========
// Common
//========

ComPointer<ID2D1SolidColorBrush> Brush::Get(ID2D1RenderTarget* target)
{
ComPointer<ID2D1SolidColorBrush> brush;
target->CreateSolidColorBrush(cColor, brush.GetObjectPointer());
return brush;
}

BOOL Brush::SetColor(COLOR c)
{
if(uColor==c)
	return false;
uColor=c;
cColor.a=(FLOAT)c.GetAlpha()/255;
cColor.b=(FLOAT)c.GetBlue()/255;
cColor.g=(FLOAT)c.GetGreen()/255;
cColor.r=(FLOAT)c.GetRed()/255;
return true;
}

}}