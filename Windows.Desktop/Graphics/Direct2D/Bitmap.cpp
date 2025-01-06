//============
// Bitmap.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Bitmap.h"
#include "ImagingFactory.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

ComPointer<ID2D1Bitmap> Bitmap::Get(ID2D1RenderTarget* target, Graphics::Bitmap* bmp)
{
D2D1_BITMAP_PROPERTIES props;
MemoryHelper::Fill(&props, sizeof(props), 0);
props.pixelFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
auto buf=bmp->Begin();
auto size=bmp->GetDimensions();
auto d2d_size=D2D1::SizeU(size.Width, size.Height);
ID2D1Bitmap* bitmap=nullptr;
target->CreateBitmap(d2d_size, buf, size.Width*4, props, &bitmap);
return bitmap;
}

}}