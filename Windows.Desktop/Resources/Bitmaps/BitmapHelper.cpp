//==================
// BitmapHelper.cpp
//==================

#include "BitmapHelper.h"


//=======
// Using
//=======

#include "Graphics/Direct2D/ImagingFactory.h"

using namespace Graphics;
using namespace Graphics::Direct2D;


//===========
// Namespace
//===========

namespace Resources {
	namespace Bitmaps {


//========
// Common
//========

Handle<Bitmap> BitmapHelper::CreateBitmap(Handle<String> path)
{
auto factory=ImagingFactory::Create();
auto source=factory->CreateBitmap(path);
UINT width=0;
UINT height=0;
source->GetSize(&width, &height);
ComPointer<IWICBitmapLock> lock;
source->Lock(nullptr, WICBitmapLockRead, lock.AddressOf());
UINT size=0;
WICInProcPointer ptr=nullptr;
lock->GetDataPointer(&size, &ptr);
assert(size==width*height*4);
UINT stride=0;
lock->GetStride(&stride);
auto bmp=Bitmap::Create(width, height, 32);
auto buf=const_cast<BYTE*>(bmp->Begin());
source->CopyPixels(nullptr, stride, size, buf);
return bmp;
}

}}