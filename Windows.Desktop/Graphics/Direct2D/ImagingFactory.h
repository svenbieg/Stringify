//==================
// ImagingFactory.h
//==================

#pragma once


//=======
// Using
//=======

#include <wincodec.h>
#include "ComPointer.h"


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
	// Con-/Destructors
	~ImagingFactory();
	static Handle<ImagingFactory> Get();

	// Common
	ComPointer<IWICBitmap> CreateBitmap(WORD Resource);
	ComPointer<IWICBitmap> CreateBitmap(HICON Icon);
	ComPointer<IWICBitmap> CreateBitmap(IWICBitmap* Source);
	ComPointer<IWICBitmap> CreateBitmap(IWICStream* Stream);
	ComPointer<IWICBitmap> CreateBitmap(Handle<String> Path);
	ComPointer<IWICBitmap> CreateBitmap(UINT Width, UINT Height);

private:
	// Con-/Destructors
	ImagingFactory();

	// Common
	ComPointer<IWICImagingFactory2> m_Factory;
	static ImagingFactory* s_Current;
};

}}