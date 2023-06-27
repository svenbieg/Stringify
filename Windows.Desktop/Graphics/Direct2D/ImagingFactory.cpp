//====================
// ImagingFactory.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include "ImagingFactory.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

HRESULT ImagingFactory::CreateBitmap(WORD id, IWICBitmap** bitmap)
{
HRSRC resource=FindResource(NULL, MAKEINTRESOURCE(id), TEXT("Image"));
if(resource==NULL)
	return E_FAIL;
HGLOBAL handle=LoadResource(NULL, resource);
if(handle==NULL)
	return E_FAIL;
VOID* ptr=LockResource(handle);
DWORD size=SizeofResource(NULL, resource);
ComPointer<IWICStream> stream;
pFactory->CreateStream(stream.GetObjectPointer());
stream->InitializeFromMemory((BYTE*)ptr, size);
HRESULT status=CreateBitmap(stream.Get(), bitmap);
UnlockResource(handle);
return status;
}

HRESULT ImagingFactory::CreateBitmap(HICON ico, IWICBitmap** bitmap)
{
return pFactory->CreateBitmapFromHICON(ico, bitmap);
}

HRESULT ImagingFactory::CreateBitmap(IWICBitmap* source, IWICBitmap** bitmap)
{
if(!source)
	return E_INVALIDARG;
UINT width=0;
UINT height=0;
source->GetSize(&width, &height);
WICPixelFormatGUID format;
source->GetPixelFormat(&format);
ComPointer<IWICBitmapLock> lock;
source->Lock(nullptr, WICBitmapLockRead, lock.GetObjectPointer());
UINT size=0;
WICInProcPointer ptr=nullptr;
lock->GetDataPointer(&size, &ptr);
UINT stride=0;
lock->GetStride(&stride);
return pFactory->CreateBitmapFromMemory(width, height, format, stride, size, ptr, bitmap);
}

HRESULT ImagingFactory::CreateBitmap(IWICStream* stream, IWICBitmap** bitmap)
{
ComPointer<IWICBitmapDecoder> decoder;
HRESULT status=pFactory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnLoad, decoder.GetObjectPointer());
if(status!=S_OK)
	return status;
ComPointer<IWICBitmapFrameDecode> frame;
status=decoder->GetFrame(0, frame.GetObjectPointer());
if(status!=S_OK)
	return status;
ComPointer<IWICFormatConverter> converter;
pFactory->CreateFormatConverter(converter.GetObjectPointer());
status=converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0., WICBitmapPaletteTypeMedianCut);
if(status!=S_OK)
	return status;
return pFactory->CreateBitmapFromSource(converter.Get(), WICBitmapNoCache, bitmap);
}

HRESULT ImagingFactory::CreateBitmap(Handle<String> path, IWICBitmap** bitmap)
{
if(!path)
	return E_INVALIDARG;
ComPointer<IWICStream> stream;
pFactory->CreateStream(stream.GetObjectPointer());
WCHAR str[MAX_PATH];
StringCopy(str, MAX_PATH, path->Begin());
HRESULT status=stream->InitializeFromFilename(str, GENERIC_READ);
if(status!=S_OK)
	return status;
return CreateBitmap(stream.Get(), bitmap);
}

HRESULT ImagingFactory::CreateBitmap(UINT width, UINT height, IWICBitmap** bitmap)
{
return pFactory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, bitmap);
}

ImagingFactory* ImagingFactory::Open()
{
if(!hCurrent)
	hCurrent=new ImagingFactory();
return hCurrent;
}


//==========================
// Con-/Destructors Private
//==========================

ImagingFactory::ImagingFactory()
{
CoInitialize(nullptr);
CoCreateInstance(CLSID_WICImagingFactory2, 0, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory2), pFactory.GetAddressOf());
}


//================
// Common Private
//================

Handle<ImagingFactory> ImagingFactory::hCurrent;

}}