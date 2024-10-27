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

class Brush
{
public:
	// Common
	static ComPointer<ID2D1SolidColorBrush> Get(ID2D1RenderTarget* Target, Graphics::Brush* Brush);
};

}}