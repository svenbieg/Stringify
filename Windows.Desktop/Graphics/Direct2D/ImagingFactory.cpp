//====================
// ImagingFactory.cpp
//====================

#include "Graphics/Direct2D/ImagingFactory.h"


//=======
// Using
//=======

#include <assert.h>


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//==================
// Con-/Destructors
//==================

ImagingFactory::~ImagingFactory()
{
s_Current=nullptr;
}

Handle<ImagingFactory> ImagingFactory::Get()
{
if(!s_Current)
	s_Current=new ImagingFactory();
return s_Current;
}


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
IWICStream* stream=nullptr;
m_Factory->CreateStream(&stream);
stream->InitializeFromMemory((BYTE*)ptr, size);
IWICBitmap* bitmap=CreateBitmap(stream);
stream->Release();
UnlockResource(handle);
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(HICON ico)
{
IWICBitmap* bitmap=nullptr;
m_Factory->CreateBitmapFromHICON(ico, &bitmap);
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(IWICBitmap* source)
{
UINT width=0;
UINT height=0;
source->GetSize(&width, &height);
WICPixelFormatGUID format;
source->GetPixelFormat(&format);
IWICBitmapLock* lock=nullptr;
source->Lock(nullptr, WICBitmapLockRead, &lock);
UINT size=0;
WICInProcPointer ptr=nullptr;
lock->GetDataPointer(&size, &ptr);
UINT stride=0;
lock->GetStride(&stride);
IWICBitmap* bitmap=nullptr;
m_Factory->CreateBitmapFromMemory(width, height, format, stride, size, ptr, &bitmap);
lock->Release();
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(IWICStream* stream)
{
IWICBitmapDecoder* decoder=nullptr;
m_Factory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnLoad, &decoder);
IWICBitmapFrameDecode* frame=nullptr;
decoder->GetFrame(0, &frame);
IWICFormatConverter* converter=nullptr;
m_Factory->CreateFormatConverter(&converter);
converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0., WICBitmapPaletteTypeMedianCut);
IWICBitmap* bitmap=nullptr;
m_Factory->CreateBitmapFromSource(converter, WICBitmapNoCache, &bitmap);
converter->Release();
frame->Release();
decoder->Release();
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(Handle<String> path)
{
IWICStream* stream=nullptr;
m_Factory->CreateStream(&stream);
#ifndef _UNICODE
WCHAR str[MAX_PATH];
StringHelper::Copy(str, MAX_PATH, path->Begin());
#else
LPCWSTR str=path->Begin();
#endif
stream->InitializeFromFilename(str, GENERIC_READ);
IWICBitmap* bitmap=CreateBitmap(stream);
stream->Release();
return bitmap;
}

ComPointer<IWICBitmap> ImagingFactory::CreateBitmap(UINT width, UINT height)
{
IWICBitmap* bitmap=nullptr;
m_Factory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &bitmap);
return bitmap;
}


//==========================
// Con-/Destructors Private
//==========================

ImagingFactory::ImagingFactory()
{
CoInitialize(nullptr);
IWICImagingFactory2* factory=nullptr;
CoCreateInstance(CLSID_WICImagingFactory2, 0, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory2), (VOID**)&factory);
m_Factory.Initialize(factory);
}


//================
// Common Private
//================

ImagingFactory* ImagingFactory::s_Current=nullptr;

}}