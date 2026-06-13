//==================
// ImagingFactory.h
//==================

#pragma once


//=======
// Using
//=======

#include <wincodec.h>
#include "ComPointer.h"
#include "Global.h"
#include "StringClass.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//=================
// Imaging-Factory
//=================

class ImagingFactory: public Global<ImagingFactory>
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<ImagingFactory> Create() { return Global::Create(); }

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
};

}}