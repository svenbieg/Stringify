//==================
// ImagingFactory.h
//==================

#pragma once


//=======
// Using
//=======

#include <wincodec.h>


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//=================
// Imaging-Factory
//=================

class ImagingFactory: public Object
{
public:
	// Common
	HRESULT CreateBitmap(WORD Resource, IWICBitmap** Bitmap);
	HRESULT CreateBitmap(HICON Icon, IWICBitmap** Bitmap);
	HRESULT CreateBitmap(IWICBitmap* Source, IWICBitmap** Bitmap);
	HRESULT CreateBitmap(IWICStream* Stream, IWICBitmap** Bitmap);
	HRESULT CreateBitmap(Handle<String> Path, IWICBitmap** Bitmap);
	HRESULT CreateBitmap(UINT Width, UINT Height, IWICBitmap** Bitmap);
	static ImagingFactory* Open();

private:
	// Con-/Destructors
	ImagingFactory();

	// Common
	static Handle<ImagingFactory> hCurrent;
	ComPointer<IWICImagingFactory2> pFactory;
};

}}