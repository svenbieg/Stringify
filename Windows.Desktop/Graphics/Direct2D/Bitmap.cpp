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

ComPointer<ID2D1Bitmap> Bitmap::Get(ID2D1RenderTarget* target)
{
ComPointer<ID2D1Bitmap> d2d_bitmap;
if(cD2DBitmaps.try_get(target, &d2d_bitmap))
	return d2d_bitmap;
D2D1_BITMAP_PROPERTIES props;
ZeroMemory(&props, sizeof(D2D1_BITMAP_PROPERTIES));
props.pixelFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
auto buf=pBitmap->Begin();
auto size=pBitmap->GetDimensions();
auto d2d_size=D2D1::SizeU(size.Width, size.Height);
target->CreateBitmap(d2d_size, buf, size.Width*4, props, d2d_bitmap.GetObjectPointer());
cD2DBitmaps.add(target, d2d_bitmap);
return d2d_bitmap;
}

ComPointer<ID2D1Bitmap> Bitmap::Get(ID2D1RenderTarget* target, Graphics::Bitmap* bitmap)
{
Handle<Bitmap> d2d_bitmap;
if(cBitmaps.try_get(bitmap, &d2d_bitmap))
	return d2d_bitmap->Get(target);
d2d_bitmap=new Bitmap(bitmap);
return d2d_bitmap->Get(target);
}


//============================
// Con-/Destructors Protected
//============================

Bitmap::Bitmap(Graphics::Bitmap* bitmap):
pBitmap(bitmap)
{
cBitmaps.add(bitmap, this);
bitmap->Destroyed.Add(this, &Bitmap::OnBitmapDestroyed);
}


//================
// Common Private
//================

VOID Bitmap::OnBitmapDestroyed(Graphics::Bitmap* bitmap)
{
cBitmaps.remove(bitmap);
}

Clusters::shared_map<Graphics::Bitmap*, Handle<Bitmap>> Bitmap::cBitmaps;

}}