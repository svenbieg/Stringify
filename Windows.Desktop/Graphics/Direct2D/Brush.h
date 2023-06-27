//=========
// Brush.h
//=========

#pragma once


//=======
// Using
//=======

#include <d2d1_2.h>
#include "Graphics/Brush.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//=======
// Brush
//=======

class Brush: public Graphics::Brush
{
public:
	// Con-/Destructors
	Brush(COLOR Color=Colors::Black);

	// Common
	ComPointer<ID2D1SolidColorBrush> Get(ID2D1RenderTarget* Target);
	COLOR GetColor()const override { return uColor; }
	BOOL SetColor(COLOR Color);

private:
	// Common
	D2D1_COLOR_F cColor;
	COLOR uColor;
};

}}