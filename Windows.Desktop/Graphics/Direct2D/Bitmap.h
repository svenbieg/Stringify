//==========
// Bitmap.h
//==========

#pragma once


//=======
// Using
//=======

#include <d2d1_2.h>
#include "Graphics/Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Bitmap
//========

class Bitmap
{
public:
	// Common
	static ComPointer<ID2D1Bitmap> Get(ID2D1RenderTarget* Target, Graphics::Bitmap* Bitmap);
};

}}