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
	ComPointer<IWICBitmap> CreateBitmap(WORD Resource);
	ComPointer<IWICBitmap> CreateBitmap(HICON Icon);
	ComPointer<IWICBitmap> CreateBitmap(IWICBitmap* Source);
	ComPointer<IWICBitmap> CreateBitmap(IWICStream* Stream);
	ComPointer<IWICBitmap> CreateBitmap(Handle<String> Path);
	ComPointer<IWICBitmap> CreateBitmap(UINT Width, UINT Height);
	static ImagingFactory* Open();

private:
	// Con-/Destructors
	ImagingFactory();

	// Common
	static Handle<ImagingFactory> m_Current;
	ComPointer<IWICImagingFactory2> m_Factory;
};

}}