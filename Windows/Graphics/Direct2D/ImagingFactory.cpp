//====================
// ImagingFactory.cpp
//====================

#include "ImagingFactory.h"


//=======
// Using
//=======

#include "ErrorHelper.h"
#include <assert.h>


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(WORD id)
{
HRSRC resource=FindResource(NULL, MAKEINTRESOURCE(id), TEXT("Image"));
assert(resource);
HGLOBAL handle=LoadResource(NULL, resource);
assert(handle);
VOID* ptr=LockResource(handle);
DWORD size=SizeofResource(NULL, resource);
ComPointer<IWICStream> stream;
m_Factory->CreateStream(stream.AddressOf());
stream->InitializeFromMemory((BYTE*)ptr, size);
auto bitmap=CreateBitmap(stream.Get());
UnlockResource(handle);
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(HICON ico)
{
ComPointer<IWICBitmap> bitmap;
m_Factory->CreateBitmapFromHICON(ico, bitmap.AddressOf());
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(IWICBitmap* source)
{
UINT width=0;
UINT height=0;
source->GetSize(&width, &height);
WICPixelFormatGUID format;
source->GetPixelFormat(&format);
ComPointer<IWICBitmapLock> lock;
source->Lock(nullptr, WICBitmapLockRead, lock.AddressOf());
UINT size=0;
WICInProcPointer ptr=nullptr;
lock->GetDataPointer(&size, &ptr);
UINT stride=0;
lock->GetStride(&stride);
ComPointer<IWICBitmap> bitmap;
m_Factory->CreateBitmapFromMemory(width, height, format, stride, size, ptr, bitmap.AddressOf());
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(IWICStream* stream)
{
ComPointer<IWICBitmapDecoder> decoder;
m_Factory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnLoad, decoder.AddressOf());
ComPointer<IWICBitmapFrameDecode> frame;
decoder->GetFrame(0, frame.AddressOf());
ComPointer<IWICFormatConverter> converter;
m_Factory->CreateFormatConverter(converter.AddressOf());
converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0., WICBitmapPaletteTypeMedianCut);
ComPointer<IWICBitmap> bitmap;
m_Factory->CreateBitmapFromSource(converter, WICBitmapNoCache, bitmap.AddressOf());
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(Handle<String> path)
{
ComPointer<IWICStream> stream;
m_Factory->CreateStream(stream.AddressOf());
#ifndef _UNICODE
WCHAR str[MAX_PATH];
StringHelper::Copy(str, MAX_PATH, path->Begin());
#else
LPCWSTR str=path->Begin();
#endif
stream->InitializeFromFilename(str, GENERIC_READ);
auto bitmap=CreateBitmap(stream.Get());
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(UINT width, UINT height)
{
ComPointer<IWICBitmap> bitmap;
m_Factory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, bitmap.AddressOf());
return bitmap;
}


//==========================
// Con-/Destructors Private
//==========================

ImagingFactory::ImagingFactory()
{
HRESULT hr=CoInitialize(nullptr);
ErrorHelper::ThrowIfFailed(hr);
CoCreateInstance(CLSID_WICImagingFactory2, 0, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory2), (VOID**)m_Factory.AddressOf());
}

}}