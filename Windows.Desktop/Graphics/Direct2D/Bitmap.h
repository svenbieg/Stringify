//==========
// Bitmap.h
//==========

#pragma once


//=======
// Using
//=======

#include <d2d1_2.h>
//#include <wincodec.h>
#include "Clusters/shared_map.hpp"
#include "Graphics/Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Bitmap
//========

class Bitmap: public Object
{
public:
	// Common
	ComPointer<ID2D1Bitmap> Get(ID2D1RenderTarget* Target);
	static ComPointer<ID2D1Bitmap> Get(ID2D1RenderTarget* Target, Graphics::Bitmap* Bitmap);

protected:
	// Con-/Destructors
	Bitmap(Graphics::Bitmap* Bitmap);

private:
	// Common
	VOID OnBitmapDestroyed(Graphics::Bitmap* Bitmap);
	static Clusters::shared_map<Graphics::Bitmap*, Handle<Bitmap>> cBitmaps;
	Clusters::shared_map<ID2D1RenderTarget*, ComPointer<ID2D1Bitmap>> cD2DBitmaps;
	Graphics::Bitmap* pBitmap;
};

}}